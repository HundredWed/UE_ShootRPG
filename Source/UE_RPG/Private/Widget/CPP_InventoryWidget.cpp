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
#include "Widget/CPP_EquipmentInventory.h"

void UCPP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::CloseWidget);
	SortButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::SortInventory);

	ACPP_Character* player = Cast<ACPP_Character>(GetOwningPlayerPawn());
	if (IsValid(player))
	{
		InventoryRef = player->GetInventory(); 
		SplitWidget->SetWeakInventoryRef(InventoryRef);
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
			UCPP_Slot* slotWidget = CreateWidget<UCPP_Slot>(this, SlotWidgetClass);
			if (IsValid(slotWidget))
			{
				SlotWidgetArray.Add(slotWidget);

				const int32 row = index / slotsParRow;
				const int32 column = index % slotsParRow;
				SlotPanel->AddChildToUniformGrid(slotWidget, row, column);

				SeSlotInfo(slotWidget, index);

				slotWidget->OnSlotDragDetected.BindUObject(this, &UCPP_InventoryWidget::OnSlotDragDetected);
				slotWidget->OnSlotDrop.BindUObject(this, &UCPP_InventoryWidget::OnSlotDrop);
				slotWidget->OnSlotMouseButtonDown.BindUObject(this, &UCPP_InventoryWidget::OnSlotMouseButtonDown);
				slotWidget->OnSlotMouseButtonDoubleClick.BindUObject(this, &UCPP_InventoryWidget::OnSlotMouseButtonDoubleClick);
				slotWidget->OnCombineDelegate.BindUObject(this, &UCPP_InventoryWidget::ChangeItem);
			}

		}

		//장비슬롯 초기화
		EquipmentInventory->InitEquipmentInventory(InventoryRef, DragWidgetClass);
	}
}

void UCPP_InventoryWidget::CloseWidget()
{
	//PlayerRef->HideGameInventory();
	SplitWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_InventoryWidget::SortInventory()
{
	if (!InventoryRef.IsValid())
	{
		return;
	}

	InventoryRef->InventorySort(0, InventoryRef->GetInventorySize() - 1);
}

void UCPP_InventoryWidget::SetPanelEnabled(bool enabled)
{
	SlotPanel->SetIsEnabled(enabled);
}

void UCPP_InventoryWidget::SetSplitWidget(const UCPP_Slot* fromSlot, const UCPP_Slot* toSlot)
{
	UUniformGridSlot* slotGrid = Cast<UUniformGridSlot>(toSlot->Slot);
	float rowSize;
	float clampRow;
	float columnSize;
	if (IsValid(slotGrid))
	{
		rowSize = (float)(slotGrid->GetRow() * SlotBoxSize) - (InventoryScrollBox->GetScrollOffset());
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

	SplitWidget->SetRenderTransform(renderTransform);
	SplitWidget->InitWidgetInfo(fromSlot->MyAmount, fromSlot->MyIndex, false, toSlot->MyIndex);
	SplitWidget->SetVisibility(ESlateVisibility::Visible);
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
	
	if (itemData)
	{
		slot->UpdateSlot(itemData, index, amount);
		SearchCombinableSlot(itemData->ItemType, index);
	}
	else
	{
		slot->InitSlotInfo(index);
	}
}

void UCPP_InventoryWidget::UpdateSlot(const FItemInfoTable* itemData, const FEquipmentInfoTable* equipmentData, const int32 index)
{
	UCPP_Slot* slot = SlotWidgetArray[index];
	slot->UpdateSlot(itemData, equipmentData, index);
}

void UCPP_InventoryWidget::CheckCombinability(const int32 toIndex, const int32 fromIndex)
{
	if (!InventoryRef.IsValid())
	{
		return;
	}

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

FName UCPP_InventoryWidget::GetCurrentEquipmentID(EEquipmentType equipmentType)
{
	return EquipmentInventory->GetEquipmentID(equipmentType);
}

void UCPP_InventoryWidget::UpdateEquipmentInventory(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo)
{
	EquipmentInventory->UpdateEquipSlot(itemInfo, equipmentInfo);
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

void UCPP_InventoryWidget::SeSlotInfo(UCPP_Slot* slot, const int32 index)
{
	if (!InventoryRef.IsValid())
	{
		return;
	}

	const FName slotItemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
	slot->InitSlotInfo(index);

	if (!slotItemID.IsNone())
	{
		const FItemInfoTable* itemData = InventoryRef->RequestItemData(slotItemID);

		if (itemData->ItemType == EItemCategory::EIC_Equipment)
		{
			const FEquipmentInfoTable* equipmentData = InventoryRef->RequestEquipmentData(slotItemID);
			slot->UpdateSlot(itemData, equipmentData, index);
		}
		else
		{
			const int32 amount = InventoryRef->GetSlotInfoIndex(index).ItemAmount;
			slot->UpdateSlot(itemData, index, amount);
			SearchCombinableSlot(itemData->ItemType, index);
		}
	}
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
					SetSplitWidget(dragSlot->WidgetRef, slot);					
				}
				else
				{
					InventoryRef->SwapSlot(fromIndex, toIndex);
				}
			}
		}
		else if (dragSlot->bFromEquipmentSlot)
		{
			TakeOffEquipment(dragSlot->EquipmentType, index);
		}

		return true;
	}
	else
	{
		return false;
	}
}

void UCPP_InventoryWidget::OnSlotMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const int32 index)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) && InventoryRef.IsValid())
	{
		const FName itemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
		const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID);
		if (itemData && itemData->ItemType != EItemCategory::EIC_Equipment)
		{
			OnUseItem(index);
		}
		else if (itemData && itemData->ItemType == EItemCategory::EIC_Equipment)
		{
			EquipSlotItem(index);
		}
	}
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
	if (!InventoryRef.IsValid())
	{
		return;
	}

	const FName itemID = InventoryRef->GetSlotInfoIndex(index).ItemID;
	if (const FItemInfoTable* itemData = InventoryRef->RequestItemData(itemID))
	{
		InventoryRef->ChangeItemInfo(itemData->CombineResultID, index);
	}	
}

void UCPP_InventoryWidget::OnUseItem(const int32 index)
{
	if (InventoryRef.IsValid())
	{
		InventoryRef->UseItem(index);
	}	
}

void UCPP_InventoryWidget::EquipSlotItem(const int32 fromIndex)
{
	if (InventoryRef.IsValid())
	{
		InventoryRef->SetEquipWeapon(fromIndex);
	}	
}

void UCPP_InventoryWidget::TakeOffEquipment(EEquipmentType equipmentType, const int32 index)
{
	if (InventoryRef->UnEquipWeaponAndAddToIndex(equipmentType, index))
	{
		EquipmentInventory->TakeOffEquipment(equipmentType);
	}
}

void UCPP_InventoryWidget::SearchCombinableSlot(EItemCategory itemType, const int32 startIndex)
{
	if (itemType != EItemCategory::EIC_Combinables || !InventoryRef.IsValid())
		return;

	SlotWidgetArray[startIndex]->CombinableSlot = InventoryRef->FindCombinableSlot(startIndex);
	const int32 combineSlot = SlotWidgetArray[startIndex]->CombinableSlot;

	if (combineSlot != -1)
	{
		SlotWidgetArray[combineSlot]->ActiveCombinableSlot();
	}

	InventoryRef->ClearConnectArray();
}




