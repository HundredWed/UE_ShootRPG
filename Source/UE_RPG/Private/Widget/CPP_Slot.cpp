// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Item.h"

void UCPP_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	SlotButton->OnClicked.AddDynamic(this, &UCPP_Slot::SlotClickEvent);
}

void UCPP_Slot::UpdateSlot(int32 index)
{
	MyArrayNumber = index;
	if (IsValid(InventoryRef))
	{
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
			FInventorySlot slotinfo = InventoryRef->GetSlotInfoIndex(index);
			const UItem* item = slotinfo.Item;
			int32 amount = slotinfo.ItemAmount;
			

			ItemIcon->SetBrushFromTexture(item->IconTexture);
			ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (item->bCanStacked)
			{
				TextAmount->SetText(FText::Format(NSLOCTEXT("CPP_Slot", "TextAmount", "x{0}"), amount));
				TextAmount->SetVisibility(ESlateVisibility::HitTestInvisible);
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

FReply UCPP_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		if (SlotButton->GetIsEnabled())
		{
			
		}
	}

	return FReply::Handled();
}



