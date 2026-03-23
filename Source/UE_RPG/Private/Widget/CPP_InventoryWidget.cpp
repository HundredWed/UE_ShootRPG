// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CPP_Character.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Widget/CPP_Slot.h"
#include "Inventory.h"
#include "Widget/DragWidget.h"
#include "Widget/CPP_DragSlotWidget.h"
#include "Widget/SlotDrag.h"
#include "Widget/SetAmountWidget.h"

void UCPP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::CloseWidget);
	SortButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::SortInventory);

	ACPP_Character* player = Cast<ACPP_Character>(GetOwningPlayerPawn());
	if (IsValid(player))
	{
		InventoryRef = player->GetInventory(); 
	}
}

void UCPP_InventoryWidget::GenerateSlotWidget(const int32 slotsParRow)
{
	if (InventoryRef.IsValid())
	{
		SlotPanel->ClearChildren();

		SlotWidgetArray.Empty();


		if (SlotWidgetClass == nullptr)
			return;
					
		for (int32 index = 0; index < InventoryRef->GetInventorySize(); index++)
		{
			SlotWidget = CreateWidget<UCPP_Slot>(this, SlotWidgetClass);
			if (IsValid(SlotWidget))
			{
				SlotWidgetArray.Add(SlotWidget);

				const int32 row = index / slotsParRow;
				const int32 column = index % slotsParRow;
				SlotPanel->AddChildToUniformGrid(SlotWidget, row, column);


				const FName slotItemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
				if (slotItemID.IsNone())
				{
					SlotWidget->InactiveSlot();
				}
				else
				{
					const FItemInfoTable* itemData = InventoryRef->RequestItemData(slotItemID);
					const int32 amount = InventoryRef->GetSlotInfoIndex(index).ItemAmount;
					SlotWidget->UpdateSlot(itemData, index, amount);
					SearchCombinableSlot(index);
				}

				SlotWidget->OnSlotDragDetected.BindUObject(this, UCPP_InventoryWidget::OnSlotDragDetected);
				SlotWidget->OnSlotDrop.BindUObject(this, UCPP_InventoryWidget::OnSlotDrop);
				SlotWidget->OnSlotMouseButtonDown.BindUObject(this, UCPP_InventoryWidget::OnSlotMouseButtonDown);
				SlotWidget->OnSlotMouseButtonDoubleClick.BindUObject(this, UCPP_InventoryWidget::OnSlotMouseButtonDoubleClick);
				SlotWidget->OnCombineDelegate.BindUObject(this, UCPP_InventoryWidget::ChangeItem);
			}

		}
	}
}

void UCPP_InventoryWidget::CloseWidget()
{
	//PlayerRef->HideGameInventory();
	SpliteWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_InventoryWidget::SortInventory()
{
	InventoryRef->InventorySort(0, InventoryRef->GetInventorySize() - 1);
}

void UCPP_InventoryWidget::SetPanelEnabled(bool enabled)
{
	SlotPanel->SetIsEnabled(enabled);
}

void UCPP_InventoryWidget::SetSpliteWidget(const UCPP_Slot* fromSlot, const UCPP_Slot* toSlot)
{
	UUniformGridSlot* slotGrid = Cast<UUniformGridSlot>(toSlot->Slot);
	float rowSize;
	float clampRow;
	float columnSize;
	if (IsValid(slotGrid))
	{
		rowSize = (float)(slotGrid->GetRow() * SlotBoxSize) - (InventoryScollBox->GetScrollOffset());
		clampRow = FMath::Clamp(rowSize, 0, InventoryBoxSize);
		columnSize = (float)(slotGrid->GetColumn() * SlotBoxSize);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UUniformGridSlot not valid!!"))
		return;
	}
	
	FVector2D render = FVector2D(columnSize, clampRow);
	FWidgetTransform renderTransform = FWidgetTransform(render,FVector2D(1,1), FVector2D::Zero(), 0);

	SpliteWidget->SetRenderTransform(renderTransform);
	SpliteWidget->InitWidgetInfo(fromSlot->MyAmount, fromSlot->MyIndex, false, toSlot->MyIndex);
	SpliteWidget->SetVisibility(ESlateVisibility::Visible);
	SetPanelEnabled(false);
}

void UCPP_InventoryWidget::UpdateWeightText(const float amount)
{
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.MinimumFractionalDigits = 1;
	FText amountText = FText::AsNumber(amount, &NumberFormatOptions);
	WeightText->SetText(FText::Format(NSLOCTEXT("UCPP_InventoryWidget", "WeightText", "{0} / {1}"), amountText, MaxWeight));
}

void UCPP_InventoryWidget::UpdateWeightMaxAmount(const float amount)
{
	MaxWeight = amount;
}

void UCPP_InventoryWidget::UpdateGoldText(const int32 amount)
{
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.UseGrouping = true;
	FText amountText = FText::AsNumber(amount, &NumberFormatOptions); 
	GoldText->SetText(FText::Format(NSLOCTEXT("UCPP_InventoryWidget", "GoldText", "{0}"), amountText));
}

void UCPP_InventoryWidget::UpdateSlot(const FItemInfoTable* itemData, const int32 index, const int32 amount)
{
	UCPP_Slot* slot = SlotWidgetArray[index];
	slot->UpdateSlot(itemData, index, amount);
	SearchCombinableSlot(index);
}

void UCPP_InventoryWidget::CheckCombinability(const int32 toIndex, const int32 fromIndex)
{
	UCPP_Slot* slot = SlotWidgetArray[toIndex];
	const FName itemID = InventoryRef->GetSlotInfoIndex(toIndex).ItemID;
	const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID);

	bool canCombine = (itemID.IsNone()) || (itemData->ItemType != EItemCategory::EIC_Combinables);

	if (canCombine && slot->LinkedCombinableSlot != -1)
	{
		SlotWidgetArray[slot->LinkedCombinableSlot]->InactiveCombinableSlot();

		slot->LinkedCombinableSlot = -1;
	}
}

FReply UCPP_InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this,EKeys::LeftMouseButton);

	return reply.NativeReply;
}

void UCPP_InventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	UDragWidget* dragWidget = Cast<UDragWidget>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragWidget::StaticClass()));

	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (IsValid(dragWidget))
	{
		dragWidget->WidgetRef = this;
		dragWidget->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		dragWidget->DefaultDragVisual = this;
		dragWidget->Pivot = EDragPivot::MouseDown;
	}
	
	OutOperation = dragWidget;
}

void UCPP_InventoryWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UDragWidget* dragWidget = Cast<UDragWidget>(InOperation);
	if (dragWidget)
	{
		RemoveFromParent();
	}
}

bool UCPP_InventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}

void UCPP_InventoryWidget::OnSlotDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation, const int32 index)
{
	if (!IsValid(DragWidgetClass) || !InventoryRef.IsValid())
		return;

	UCPP_Slot* slot = SlotWidgetArray[index];
	const FName itemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
	const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID);

	UCPP_DragSlotWidget* dragWidget = CreateWidget<UCPP_DragSlotWidget>(GetWorld(), DragWidgetClass);
	if (IsValid(dragWidget) && itemData)
		dragWidget->UpdataWidget(itemData, slot->MyAmount);

	USlotDrag* dragSlot = Cast<USlotDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDrag::StaticClass()));

	if (IsValid(dragSlot))
	{
		if (slot->LinkedCombinableSlot != -1)
		{
			SlotWidgetArray[slot->LinkedCombinableSlot]->InactiveCombinableSlot();
		}

		dragSlot->WidgetRef = slot;
		dragSlot->DefaultDragVisual = dragWidget;
		dragSlot->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = dragSlot;
}

bool UCPP_InventoryWidget::OnSlotDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation, const int32 index)
{
	USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
	UCPP_Slot* slot = SlotWidgetArray[index];

	if (dragSlot && InventoryRef.IsValid())
	{
		if (dragSlot->WidgetRef && dragSlot->WidgetRef != slot)
		{
			const int16 fromIndex = dragSlot->WidgetRef->MyIndex;
			const int16 toIndex = index;

			slot->bDraggedOver = false;

			//border
			slot->SetBorder();

			if (InventoryRef->CanAddToIndex(fromIndex, toIndex))
			{
				InventoryRef->AddToIndex(fromIndex, toIndex);;
			}
			else
			{
				if (InDragDropEvent.IsShiftDown())
				{
					SetSpliteWidget(dragSlot->WidgetRef, slot);					
				}
				else
				{
					InventoryRef->SwapSlot(fromIndex, toIndex);
				}
			}
		}
		else if (dragSlot->bFromEquipmentSlot)
		{
			InventoryRef->UnEquipWeaponAndAddItem(index);			
		}

		return true;
	}
	else
	{
		return false;
	}
}

FReply UCPP_InventoryWidget::OnSlotMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const int32 index)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && InventoryRef.IsValid())
	{
		const FName itemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
		const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID);
		if (itemData && itemData->ItemType != EItemCategory::EIC_Equipment)
		{
			OnUseItem(index);
		}
		else if(itemData && itemData->ItemType == EItemCategory::EIC_Equipment)
		{
			EquipSlotItem(itemID, index);
		}

		return FReply::Handled();
	}
	else
	{
		FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return reply.NativeReply;
	}

	return FReply::Unhandled();
}

FReply UCPP_InventoryWidget::OnSlotMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const int32 index)
{
	const FName itemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
	
	if (const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID))
	{
		if (InventoryRef.IsValid() && itemData->ItemType != EItemCategory::EIC_Equipment)
		{
			OnUseItem(index);
		}
		else
		{
			return FReply::Handled();
		}
	}	

	return FReply::Unhandled();
}

void UCPP_InventoryWidget::ChangeItem(const int32 index)
{
	const FName itemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
	if (const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID))
	{
		InventoryRef->ChangeItemInfo(itemData->CombineResultID, index);
	}	
}

void UCPP_InventoryWidget::OnUseItem(const int32 index)
{
	InventoryRef->UseItem(index);
}

void UCPP_InventoryWidget::EquipSlotItem(const FName& equipmentID, const int32 index)
{
	InventoryRef->SetEquipWeapon(equipmentID, index);
}

void UCPP_InventoryWidget::SearchCombinableSlot(const int32 startIndex)
{

}




