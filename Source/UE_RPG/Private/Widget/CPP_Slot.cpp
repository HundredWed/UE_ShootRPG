// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory.h"
#include "CPP_Character.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Item.h"
#include "Item/ItemAbility.h"
#include "Widget/SlotDrag.h"
#include "Widget/CPP_DragSlotWidget.h"
#include "Widget/SetAmountWidget.h"

void UCPP_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this, &UCPP_Slot::SlotClickEvent);
}

void UCPP_Slot::UpdateSlot(const uint8 index)
{
	if (IsValid(InventoryRef))
	{
		MyArrayNumber = index;
		bool isSlotEmpty = InventoryRef->IsSlotEmpty(index);

		if (isSlotEmpty)
		{
			SlotButton->SetIsEnabled(false);
			ItemIcon->SetVisibility(ESlateVisibility::Hidden);
			TextAmount->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			SlotButton->SetIsEnabled(true);
			const FInventorySlot slotinfo = InventoryRef->GetSlotInfoIndex(index);
			const UItem* item = slotinfo.Item;
			MyAmount = slotinfo.ItemAmount;
			bMyItemCanStacked = item->bCanStacked;

			ItemIcon->SetBrushFromTexture(item->IconTexture);
			ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (item->bCanStacked)
			{
				TextAmount->SetText(FText::Format(NSLOCTEXT("CPP_Slot", "TextAmount", "x{0}"), MyAmount));
				TextAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				TextAmount->SetVisibility(ESlateVisibility::Hidden);
			}

		}
	}
}

void UCPP_Slot::SlotClickEvent()
{
	ClickCount += 1;
	FTimerHandle TimerHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCPP_Slot::ResetCount, 0.17f);

	if (ClickCount > 1)
	{
		/**Equip weapon*/

		ResetCount();
	}
}

void UCPP_Slot::ResetCount()
{
	ClickCount = 0;
}

FReply UCPP_Slot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (SlotButton->GetIsEnabled())
	{
		FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return reply.NativeReply;
	}


	return FReply::Handled();
}

void UCPP_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UCPP_DragSlotWidget* dragWidget = CreateWidget<UCPP_DragSlotWidget>(GetWorld(), DragWidgetClass);
	const FInventorySlot slotinfo = InventoryRef->GetSlotInfoIndex(MyArrayNumber);
	const UItem* item = slotinfo.Item;
	const uint32 amount = MyAmount;
	dragWidget->UpdataWidget(item, amount);

	USlotDrag* dragSlot = Cast<USlotDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDrag::StaticClass()));

	if (IsValid(dragSlot))
	{
		dragSlot->WidgetRef = this;
		dragSlot->DefaultDragVisual = dragWidget;
		dragSlot->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = dragSlot;
}

bool UCPP_Slot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		return true;
	}
	else
	{
		USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
		if (dragSlot)
		{
			UE_LOG(LogTemp, Display, TEXT("DragOver "));
			bDraggedOver = true;
			SlotButton->SetStyle(OverStlyle);
			return true;
		}
		else
		{
			return false;
		}
	}
	
}

void UCPP_Slot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	if (bDraggedOver)
	{
		USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
		if (dragSlot)
		{
			bDraggedOver = false;
			SlotButton->SetStyle(DefaultStlyle);
		}
	}
}

bool UCPP_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);

	if (dragSlot)
	{
		const uint8 fromIndex = dragSlot->WidgetRef->MyArrayNumber;
		const uint8 toIndex = MyArrayNumber;
		if (dragSlot->WidgetRef != this)
		{
			bDraggedOver = false;

			SlotButton->SetStyle(DefaultStlyle);

			if (InventoryRef->CanAddToIndex(fromIndex, toIndex))
			{
				InventoryRef->AddToIndex(fromIndex, toIndex);
				return true;
			}
			else
			{
				if (PlayerRef->bShiftDown)
				{
					InventoryRef->InventoryWidget->SetSpliteWidget(dragSlot->WidgetRef, this);
					return true;
				}
				else
				{
					InventoryRef->SwapSlot(fromIndex, toIndex);
					return true;
				}
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

FReply UCPP_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (SlotButton->GetIsEnabled())
		{
			if (IsValid(InventoryRef))
			{
				OnUseItem(InventoryRef);
			}
		}
	}

	return FReply::Handled();
}

FReply UCPP_Slot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (SlotButton->GetIsEnabled())
		{
			if (IsValid(InventoryRef))
			{
				OnUseItem(InventoryRef);
			}
		}
	}

	return FReply::Handled();
}



void UCPP_Slot::OnUseItem(class UInventory* inventory)
{
	AActor* item = GetWorld()->SpawnActor(inventory->SlotsArray[MyArrayNumber].Item->ItemClass);

	IItemAbility* itemAbility = Cast<IItemAbility>(item);
	if (itemAbility)
	{
		const uint32 value = inventory->SlotsArray[MyArrayNumber].Item->ConsumeValue;
		itemAbility->UseItem(PlayerRef, value);
	}
	item->Destroy();
	inventory->RemoveItemAtIndex(MyArrayNumber, 1);
}

