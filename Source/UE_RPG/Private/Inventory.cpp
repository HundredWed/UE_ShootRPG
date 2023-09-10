#include "Inventory.h"
#include "Item/Item.h"
#include "CPP_Character.h"
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

void UInventory::AddItem(UItem* item, const uint32 amount)
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
			const uint32 amountOver = SlotsArray[canStackedSlotIndex].ItemAmount + amount;
			if (amountOver > MaxStackSize)
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = MaxStackSize;

				/**update widget*/
				UpdateSlotAtIndex(canStackedSlotIndex);

				const uint32 restAmountOver = amountOver - MaxStackSize;
				AddItem(item, restAmountOver);

				return;
			}
			else
			{
				SlotsArray[canStackedSlotIndex].Item = item;
				SlotsArray[canStackedSlotIndex].ItemAmount = amountOver;

				/**update widget*/
				UpdateSlotAtIndex(canStackedSlotIndex);

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
					UpdateSlotAtIndex(emptySlotForStack);

					const uint32 restAmount = amount - MaxStackSize;
					AddItem(item, restAmount);

					return;
				}
				else
				{
					SlotsArray[emptySlotForStack].Item = item;
					SlotsArray[emptySlotForStack].ItemAmount = amount;

					/**update widget*/
					UpdateSlotAtIndex(emptySlotForStack);

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
		UpdateSlotAtIndex(emptySlot);


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



void UInventory::RemoveItemAtIndex(const uint8 index, const uint32 removeAmount)
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

void UInventory::SwapSlot(const uint8 fromIndex, const uint8 toIndex)
{
	const uint8 lastSlot = SlotsArray.Num() - 1;

	if (fromIndex > lastSlot || toIndex > lastSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("not valid slot!!"));
		return;
	}
	else if(IsSlotEmpty(toIndex))
	{
		UItem* item = SlotsArray[fromIndex].Item;
		SlotsArray[toIndex].Item = item;
		SlotsArray[toIndex].ItemAmount = SlotsArray[fromIndex].ItemAmount;

		SlotsArray[fromIndex].Item = nullptr;
		SlotsArray[fromIndex].ItemAmount = 0;

		UpdateSlotAtIndex(fromIndex);
		UpdateSlotAtIndex(toIndex);
	}
	else
	{
		FInventorySlot tempSlot = SlotsArray[toIndex];
		SlotsArray[toIndex] = SlotsArray[fromIndex];
		SlotsArray[fromIndex] = tempSlot;

		UpdateSlotAtIndex(fromIndex);
		UpdateSlotAtIndex(toIndex);
	}
}

void UInventory::AddToIndex(const uint8 fromIndex, const uint8 toIndex)
{
	if (CanAddToIndex(fromIndex, toIndex))
	{
		const uint8 restAmount = MaxStackSize - SlotsArray[toIndex].ItemAmount;
		if (restAmount >= SlotsArray[fromIndex].ItemAmount)
		{
			const uint8 addAmount = SlotsArray[fromIndex].ItemAmount + SlotsArray[toIndex].ItemAmount;

			/**set slot-toIndex*/
			SlotsArray[toIndex].ItemAmount = addAmount;

			/**set slot-fromIndex*/
			SlotsArray[fromIndex].Item = nullptr;
			SlotsArray[fromIndex].ItemAmount = 0;

			UpdateSlotAtIndex(fromIndex);
			UpdateSlotAtIndex(toIndex);
		}
		else
		{
			/**set slot-fromIndex*/
			SlotsArray[fromIndex].ItemAmount = SlotsArray[fromIndex].ItemAmount - restAmount;

			/**set slot-toIndex*/
			SlotsArray[toIndex].ItemAmount = MaxStackSize;

			UpdateSlotAtIndex(fromIndex);
			UpdateSlotAtIndex(toIndex);
		}
	}
	else
	{
		return;
	}

}

bool UInventory::CanAddToIndex(const uint8 fromIndex, const uint8 toIndex)
{
	if (IsSlotEmpty(toIndex))
	{
		return false;
	}

	FName fromIndexItemId = SlotsArray[fromIndex].Item->ItemInfoID;
	FName toIndexItemId = SlotsArray[toIndex].Item->ItemInfoID;

	bool brestAmount = SlotsArray[toIndex].ItemAmount < MaxStackSize;
	bool bstacked = SlotsArray[toIndex].Item->bCanStacked;

	return (fromIndexItemId == toIndexItemId) && brestAmount && bstacked;
}


void UInventory::SplitStackToIndex(const uint8 fromIndex, const uint8 toIndex, const int32 splitAmount)
{
	if (CanSplitStakable(fromIndex, toIndex, splitAmount))
	{
		SlotsArray[fromIndex].ItemAmount -= splitAmount;

		UItem* fromIndexItem = SlotsArray[fromIndex].Item;
		SlotsArray[toIndex].Item = fromIndexItem;
		SlotsArray[toIndex].ItemAmount = splitAmount;

		UpdateSlotAtIndex(fromIndex);
		UpdateSlotAtIndex(toIndex);
	}
	else
	{
		return;
	}


}

bool UInventory::CanSplitStakable(const uint8 fromIndex, const uint8 toIndex, const int32 splitAmount)
{
	return IsSlotEmpty(toIndex)
		&& (SlotsArray[fromIndex].Item->bCanStacked) 
		&& (SlotsArray[fromIndex].ItemAmount > 1) 
		&& (SlotsArray[fromIndex].ItemAmount > splitAmount);
}

void UInventory::UpdateSlotAtIndex(const uint8 index)
{
	InventoryWidget->SlotWidgetArray[index]->UpdateSlot(index);
}

FInventorySlot UInventory::GetSlotInfoIndex(const uint8 index)
{
	return SlotsArray[index];
}






