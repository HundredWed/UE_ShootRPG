#include "Inventory.h"
#include "Item/Item.h"

UInventory::UInventory()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UInventory::BeginPlay()
{
	Super::BeginPlay();

	
	
}



void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

bool UInventory::IsSlotEmpty(int32 index)
{
	AItem* item = SlotsArray[index].Item;

	if (IsValid(item))
	{
		return true;
	}

	return false;
	
}

void UInventory::AddItem(AItem* item, int32 amount)
{
	FItemInfo itemInfo = item->GetItemInfo();

	/**item is Stacked?*/
	if (itemInfo.bCanStacked)
	{
		/**can Stacked item*/

		int32 canStackedSlotIndex;
		if (SearchFreeStackSlot(item, canStackedSlotIndex))
		{
			/**found can stack slot*/
			int32 amountOver = SlotsArray[canStackedSlotIndex].ItemAmount + amount;
			if (amountOver > MaxStackSize)
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = MaxStackSize;

				/**update widget*/
				//...

				int32 restAmountOver = amountOver - MaxStackSize;
				AddItem(item, restAmountOver);

				return;
			}
			else
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = amountOver;

				/**update widget*/
				//...

				return;

			}
		}
		else
		{
			/**not found can stack slot*/
			int32 emptySlot;
			if (SearchEmptySlot(emptySlot))
			{
				if (amount > MaxStackSize)
				{
					SlotsArray[emptySlot].Item = item;
					SlotsArray[emptySlot].ItemAmount = MaxStackSize;

					/**update widget*/
					//...

					int32 restAmount = amount - MaxStackSize;
					AddItem(item, restAmount);

					return;
				}
				else
				{
					SlotsArray[emptySlot].Item = item;
					SlotsArray[emptySlot].ItemAmount = amount;

					/**update widget*/
					//...

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
		//...


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

bool UInventory::SearchFreeStackSlot(class AItem* item, int32& canStackedSlotIndex)
{
	for (int32 index = 0; index < SlotsArray.Num(); index++)
	{
		AItem* slotItem = SlotsArray[index].Item;
		int32 slotItemAmount = SlotsArray[index].ItemAmount;
		if (slotItem != nullptr)
		{
			if (slotItem == item && slotItemAmount < MaxStackSize)
			{
				canStackedSlotIndex = index;
				return true;
			}
		}
	}

	/**not enough invevtory slot!! */
	return false;
}





