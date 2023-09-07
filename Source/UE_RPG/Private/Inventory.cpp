#include "Inventory.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Widget/MainPanelWidget.h"
#include "Widget/CPP_Slot.h"

UInventory::UInventory()
{
	
	PrimaryComponentTick.bCanEverTick = false;

}


void UInventory::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ACPP_Character>(GetOwner());
	if (PlayerRef)
	{
		SlotsArray.SetNum(PlayerRef->GetInventorySize());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not found PlayerRef at Inventory!!"));
	}

	if (MainPanelclass)
	{
		MainPanelWidget = CreateWidget<UMainPanelWidget>(GetWorld(), MainPanelclass);
		InventoryWidget = MainPanelWidget->GetInventoryWidget();
		MainPanelWidget->AddToViewport();
	}
	
}



bool UInventory::IsSlotEmpty(const uint8 index)
{
	UItem* item = SlotsArray[index].Item;

	if (item == nullptr)
	{
		return true;
	}

	return false;
	
}

void UInventory::AddItem(UItem* item, const uint8 amount)
{
	if (!IsValid(item))
	{
		return;
	}

	/**item is Stacked?*/
	if (item->bCanStacked)
	{
		/**can Stacked item*/

		uint8 canStackedSlotIndex;
		if (SearchFreeStackSlot(item, canStackedSlotIndex))
		{
			/**found can-stack slot*/
			const uint8 amountOver = SlotsArray[canStackedSlotIndex].ItemAmount + amount;
			if (amountOver > MaxStackSize)
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = MaxStackSize;

				/**update widget*/
				InventoryWidget->SlotWidgetArray[canStackedSlotIndex]->UpdateSlot(canStackedSlotIndex);

				const uint8 restAmountOver = amountOver - MaxStackSize;
				AddItem(item, restAmountOver);

				return;
			}
			else
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = amountOver;

				/**update widget*/
				InventoryWidget->SlotWidgetArray[canStackedSlotIndex]->UpdateSlot(canStackedSlotIndex);

				return;

			}
		}
		else
		{
			/**not found can stack slot*/
			uint8 emptySlotForStack;
			if (SearchEmptySlot(emptySlotForStack))
			{
				if (amount > MaxStackSize)
				{
					SlotsArray[emptySlotForStack].Item = item;
					SlotsArray[emptySlotForStack].ItemAmount = MaxStackSize;

					/**update widget*/
					InventoryWidget->SlotWidgetArray[emptySlotForStack]->UpdateSlot(emptySlotForStack);

					const uint8 restAmount = amount - MaxStackSize;
					AddItem(item, restAmount);

					return;
				}
				else
				{
					SlotsArray[emptySlotForStack].Item = item;
					SlotsArray[emptySlotForStack].ItemAmount = amount;

					/**update widget*/
					InventoryWidget->SlotWidgetArray[emptySlotForStack]->UpdateSlot(emptySlotForStack);

					return;
				}
			}
			else
			{
				return;
			}
		}
		
	}
	else
	{
		/**can't Stacked item*/

		uint8 emptySlot;
		const uint8 defaultAmount = 1;
		if (SearchEmptySlot(emptySlot))
		{
			SlotsArray[emptySlot].Item = item;
			SlotsArray[emptySlot].ItemAmount = defaultAmount;
		}
		else
		{
			return;
		}

		/**update widget*/
		InventoryWidget->SlotWidgetArray[emptySlot]->UpdateSlot(emptySlot);


		/**In the case of acquiring multiple 'can't Stacked items' */
		if (amount > defaultAmount)
		{
			const uint8 addRestItem = amount - 1;
			AddItem(item, addRestItem);
		}
		else
		{
			return;
		}
	}

}

bool UInventory::SearchEmptySlot(uint8& emptySlotIndex)
{
	for (int32 index = 0; index < SlotsArray.Num(); index++)
	{
		if (SlotsArray[index].Item == nullptr)
		{
			emptySlotIndex = index;
			return true;
		}
	}

	/**not enough invevtory slot!! */
	return false;
}

bool UInventory::SearchFreeStackSlot(class UItem* item, uint8& canStackedSlotIndex)
{
	for (int32 index = 0; index < SlotsArray.Num(); index++)
	{
		UItem* slotItem = SlotsArray[index].Item;
		const uint8 slotItemAmount = SlotsArray[index].ItemAmount;
		if (slotItem != nullptr)
		{
			if ((slotItem->ItemInfoID == item->ItemInfoID) && (slotItemAmount < MaxStackSize))
			{
				canStackedSlotIndex = index;
				return true;
			}
		}
	}

	/**not enough invevtory slot!! */
	return false;
}

int32 UInventory::GetAmountAtIndex(const uint8 index)
{
	return SlotsArray[index].ItemAmount;
}

void UInventory::RemoveItemAtIndex(const uint8 index, const uint8 removeAmount)
{
	if (!IsSlotEmpty(index) && (removeAmount > 0))
	{
		const uint8 amount = GetAmountAtIndex(index);
		if (removeAmount >= amount)
		{
			SlotsArray[index].Item = nullptr;
			SlotsArray[index].ItemAmount = 0;
			InventoryWidget->SlotWidgetArray[index]->UpdateSlot(index);
			return;
		}
		else
		{
			SlotsArray[index].ItemAmount = amount - removeAmount;
			InventoryWidget->SlotWidgetArray[index]->UpdateSlot(index);
			return;
		}
		
	}

	return;
}

FInventorySlot UInventory::GetSlotInfoIndex(const uint8 index)
{
	return SlotsArray[index];
}





