// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_Slot.h"
#include "Inventory.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Item.h"

void UCPP_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCPP_Slot::UpdateSlot(int32 index)
{
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
			AItem* item = slotinfo.Item;
			int32 amount = slotinfo.ItemAmount;
			

			ItemIcon->SetBrushFromTexture(item->GetItemInfo().IconTexture);
			ItemIcon->SetVisibility(ESlateVisibility::HitTestInvisible);

			if (item->GetItemInfo().bCanStacked)
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


