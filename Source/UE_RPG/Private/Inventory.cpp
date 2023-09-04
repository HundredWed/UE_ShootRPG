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



bool UInventory::IsSlotEmpty(int32 index)
{
	UItem* item = SlotsArray[index].Item;

	if (item == nullptr)
	{
		return true;
	}

	return false;
	
}

void UInventory::AddItem(UItem* item, int32 amount)
{
	if (!IsValid(item))
	{
		return;
	}

	/**item is Stacked?*/
	if (item->bCanStacked)
	{
		/**can Stacked item*/

		int32 canStackedSlotIndex;
		if (SearchFreeStackSlot(item, canStackedSlotIndex))
		{
			/**found can-stack slot*/
			int32 amountOver = SlotsArray[canStackedSlotIndex].ItemAmount + amount;
			if (amountOver > MaxStackSize)
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = MaxStackSize;

				/**update widget*/
				InventoryWidget->SlotWidgetArray[canStackedSlotIndex]->UpdateSlot(canStackedSlotIndex);

				int32 restAmountOver = amountOver - MaxStackSize;
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
			int32 emptySlotForStack;
			if (SearchEmptySlot(emptySlotForStack))
			{
				if (amount > MaxStackSize)
				{
					SlotsArray[emptySlotForStack].Item = item;
					SlotsArray[emptySlotForStack].ItemAmount = MaxStackSize;

					/**update widget*/
					InventoryWidget->SlotWidgetArray[emptySlotForStack]->UpdateSlot(emptySlotForStack);

					int32 restAmount = amount - MaxStackSize;
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

		int32 emptySlot;
		const int32 defaultAmount = 1;
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
			int32 addRestItem = amount - 1;
			AddItem(item, addRestItem);
		}
		else
		{
			return;
		}
	}

}

bool UInventory::SearchEmptySlot(int32& emptySlotIndex)
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

bool UInventory::SearchFreeStackSlot(class UItem* item, int32& canStackedSlotIndex)
{
	for (int32 index = 0; index < SlotsArray.Num(); index++)
	{
		UItem* slotItem = SlotsArray[index].Item;
		int32 slotItemAmount = SlotsArray[index].ItemAmount;
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

int32 UInventory::GetAmountAtIndex(int32 index)
{
	return SlotsArray[index].ItemAmount;
}

void UInventory::RemoveItemAtIndex(const int32 index, const int32 removeAmount)
{
	if (!IsSlotEmpty(index) && (removeAmount > 0))
	{
		int32 amount = GetAmountAtIndex(index);
		if (removeAmount >= amount)
		{
			SlotsArray[index].Item = nullptr;
			SlotsArray[index].ItemAmount = 0;
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

FInventorySlot UInventory::GetSlotInfoIndex(const int32 index)
{
	return SlotsArray[index];
}





