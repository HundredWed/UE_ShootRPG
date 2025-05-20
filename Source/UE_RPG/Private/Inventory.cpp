#include "Inventory.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Widget/CPP_Slot.h"
#include "Widget/CPP_EquipmentInventory.h"
#include "Widget/CPP_EquipSlot.h"
#include "Item/PickUpItem.h"

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

		isConect.SetNum(PlayerRef->GetInventorySize());
		ClearConectArray();

		InvetoryRow = PlayerRef->GetInventoryRowSize();

		MaxWeight = PlayerRef->GetPlayerWeightInfo();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not found PlayerRef at Inventory!!"));
	}
	
}

bool UInventory::IsSlotEmpty(const int16 index)
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
	if (item->ItemInfoTable.bCanStacked)
	{
		/**can Stacked item*/

		int16 canStackedSlotIndex;
		if (SearchFreeStackSlot(item, canStackedSlotIndex))
		{
			/**found can-stack slot*/

			const uint32 amountOver = SlotsArray[canStackedSlotIndex].ItemAmount + amount;
			if (amountOver > MaxStackSize)
			{
				const float restWeight = MaxStackSize - SlotsArray[canStackedSlotIndex].ItemAmount;

				/**update widget*/
				UpdateInventory(canStackedSlotIndex, item, MaxStackSize);
				AddWeight(item->ItemInfoTable.Weight * restWeight);

				const uint32 restAmountOver = amountOver - MaxStackSize;
				AddItem(item, restAmountOver);

				return;
			}
			else
			{
				/**update widget*/
				UpdateInventory(canStackedSlotIndex, item, amountOver);
				AddWeight(item->ItemInfoTable.Weight * amount);
				return;

			}
		}
		else
		{
			/**not found can stack slot*/
			int16 emptySlotToStack;
			if (SearchEmptySlot(emptySlotToStack))
			{
				if (amount > MaxStackSize)
				{
					/**update widget*/
					UpdateInventory(emptySlotToStack, item, MaxStackSize);
					AddWeight(item->ItemInfoTable.Weight * MaxStackSize);

					const uint32 restAmount = amount - MaxStackSize;
					AddItem(item, restAmount);

					return;
				}
				else
				{
					/**update widget*/
					UpdateInventory(emptySlotToStack, item, amount);
					AddWeight(item->ItemInfoTable.Weight * amount);

					return;
				}
			}
			else
			{
				return;
			}
		}
		
	}
	else /**can't Stacked item*/
	{
		int16 emptySlot;
		const int16 defaultAmount = 1;
		if (SearchEmptySlot(emptySlot))
		{
			/**update widget*/
			UpdateInventory(emptySlot, item, defaultAmount);
			AddWeight(item->ItemInfoTable.Weight);
		}
		else
		{
			return;
		}

		/**In the case of acquiring multiple "can't Stacked items" */
		if (amount > defaultAmount)
		{
			const int32 addRestItem = amount - 1;
			UItem* copyItem = item->CreateItemCopy();
			AddItem(copyItem, addRestItem);
		}
		else
		{
			return;
		}
	}

}

bool UInventory::SearchEmptySlot(int16& emptySlotIndex)
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

bool UInventory::SearchFreeStackSlot(class UItem* item, int16& canStackedSlotIndex)
{
	for (int32 index = 0; index < SlotsArray.Num(); index++)
	{
		UItem* slotItem = SlotsArray[index].Item;
		const uint8 slotItemAmount = SlotsArray[index].ItemAmount;
		if (slotItem != nullptr)
		{
			if ((slotItem->ItemInfoTable.ItemInfoID == item->ItemInfoTable.ItemInfoID) && (slotItemAmount < MaxStackSize))
			{
				canStackedSlotIndex = index;
				return true;
			}
		}
	}

	/**not enough invevtory slot!! */
	return false;
}

int32 UInventory::GetAmountAtIndex(const int16 index)
{
	return SlotsArray[index].ItemAmount;
}


void UInventory::RemoveItemAtIndex(const int16 index, const int32 removeAmount)
{
	if (!IsSlotEmpty(index) && (removeAmount > 0))
	{
		const int32 amount = GetAmountAtIndex(index);
		const float weight = SlotsArray[index].Item->ItemInfoTable.Weight;
		if (removeAmount >= amount)
		{ 
			UpdateInventory(index, nullptr, 0);			
		}
		else
		{
			SlotsArray[index].ItemAmount = amount - removeAmount;
			UpdateSlotAtIndex(index);
		}

		AddWeight(-(weight * removeAmount));
	}

	return;
}

void UInventory::SwapSlot(const int16 fromIndex, const int16 toIndex)
{
	CheckItemType(fromIndex, toIndex);
	CheckItemType(toIndex, fromIndex);
		
	const int16 lastSlot = SlotsArray.Num() - 1;

	if (fromIndex > lastSlot || toIndex > lastSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("not valid slot!!"));
		return;
	}
	/*else if(IsSlotEmpty(toIndex))
	{
		UItem* item = SlotsArray[fromIndex].Item;
		UpdateInventory(toIndex, item, SlotsArray[fromIndex].ItemAmount);
		UpdateInventory(fromIndex, nullptr, 0);
		
	}*/
	else
	{
		FInventorySlot tempSlot = SlotsArray[toIndex];
		SlotsArray[toIndex] = SlotsArray[fromIndex];
		SlotsArray[fromIndex] = tempSlot;

		UpdateSlotAtIndex(toIndex);
		UpdateSlotAtIndex(fromIndex);
		
	}
}

void UInventory::CheckItemType(const int16 fromIndex, const int16 toIndex)
{
	if (SlotsArray[toIndex].Item 
		&& SlotsArray[toIndex].Item->ItemInfoTable.ItemType == EItemCategory::EIS_Combinables)
	{
		InventoryWidget->SlotWidgetArray[toIndex]->CheckCombinability(fromIndex);
	}
}

void UInventory::AddToIndex(const int16 fromIndex, const int16 toIndex)
{
	if (CanAddToIndex(fromIndex, toIndex))
	{
		const int32 restAmount = MaxStackSize - SlotsArray[toIndex].ItemAmount;
		if (restAmount >= SlotsArray[fromIndex].ItemAmount)
		{
			const int32 addAmount = SlotsArray[fromIndex].ItemAmount + SlotsArray[toIndex].ItemAmount;

			/**set slot-toIndex*/
			SlotsArray[toIndex].ItemAmount = addAmount;

			/**set slot-fromIndex*/
			UpdateInventory(fromIndex, nullptr, 0);
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

bool UInventory::CanAddToIndex(const int16 fromIndex, const int16 toIndex)
{
	if (IsSlotEmpty(toIndex))
	{
		return false;
	}

	FName fromIndexItemId = SlotsArray[fromIndex].Item->ItemInfoTable.ItemInfoID;
	FName toIndexItemId = SlotsArray[toIndex].Item->ItemInfoTable.ItemInfoID;

	bool brestAmount = SlotsArray[toIndex].ItemAmount < MaxStackSize;
	bool bstacked = SlotsArray[toIndex].Item->ItemInfoTable.bCanStacked;

	return (fromIndexItemId == toIndexItemId) && brestAmount && bstacked;
}

void UInventory::UpdateInventory(int16 index, UItem* item, int32 amount)
{
	SlotsArray[index].Item = item;
	SlotsArray[index].ItemAmount = amount;

	UpdateSlotAtIndex(index);
}


void UInventory::SplitStackToIndex(const int16 fromIndex, const int16 toIndex, const int32 splitAmount)
{
	if (CanSplitStakable(fromIndex, toIndex, splitAmount))
	{
		SlotsArray[fromIndex].ItemAmount -= splitAmount;
		UItem* fromIndexItem = SlotsArray[fromIndex].Item;

		UpdateSlotAtIndex(fromIndex);
		UpdateInventory(toIndex, fromIndexItem, splitAmount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not SplitStackToIndex!!"));
		return;
	}


}

bool UInventory::CanSplitStakable(const int16 fromIndex, const int16 toIndex, const int32 splitAmount)
{
	return IsSlotEmpty(toIndex)
		&& (SlotsArray[fromIndex].Item->ItemInfoTable.bCanStacked)
		&& (SlotsArray[fromIndex].ItemAmount > 1) 
		&& (SlotsArray[fromIndex].ItemAmount > splitAmount);
}

void UInventory::UpdateSlotAtIndex(const int16 index)
{
	InventoryWidget->SlotWidgetArray[index]->UpdateSlot(index);
}

const FInventorySlot UInventory::GetSlotInfoIndex(const int16 index)
{
	return SlotsArray[index];
}

void UInventory::AddWeight(const float amount)
{
	CurrnetWeight += amount;
	InventoryWidget->UpdateWeightText(CurrnetWeight);
}

void UInventory::AddGold(const int32 amount)
{
	CurrentGold += amount;
	InventoryWidget->UpdateGoldText(CurrentGold);
}

int16 UInventory::FindCombinableSlot(const int16 slot)
{
	isConect[slot] = true;
	int16 count = 0;
	int16 resultSlot = 0;
	int16 dir[4] = { -InvetoryRow, 1, InvetoryRow, -1 };

	if (IsLineChange(slot) == false)
	{
		for (int16 i = 0; i < 4; i++)
		{
			int16 newdir = slot + dir[i];
			if ((newdir < 0) || (newdir > (isConect.Num() - 1)) || !CompaireID(slot, newdir))
			{
				continue;
			}

			if (CompaireID(slot, newdir))
			{
				/**when drag event linked slot, inactive combine-button*/
				if (SetLinkSlot(slot, newdir))
				{
					/**is Success?*/
					count += 1;
				}
			}
		}
	}


	if (count == 4)
	{
		/**when drag event linked slot, inactive combine-button*/
		SetLinkSlot(slot, slot);
		return slot;
	}

	for (uint8 i = 0; i < 4; i++)
	{
		int16 newdir = slot + dir[i];
		
		if ((newdir < 0) || (newdir > (isConect.Num() - 1)) || !CompaireID(slot, newdir) || (isConect[newdir] == true))
		{
			continue;
		}

		if (CompaireID(slot, newdir) && (isConect[newdir] == false))
		{
			resultSlot = FindCombinableSlot(newdir);

			if (resultSlot != -1)
			{
				return resultSlot;
			}
		}
	}
	
	SetLinkSlot(-1, slot);
	return -1;
}

bool UInventory::CompaireID(const int16 slot1, const int16 slot2)
{
	if (SlotsArray[slot2].Item == nullptr)
	{
		return false;
	}

	return SlotsArray[slot1].Item->ItemInfoTable.ItemInfoID == SlotsArray[slot2].Item->ItemInfoTable.ItemInfoID;
}

void UInventory::ClearConectArray()
{
	isConect.Init(false, PlayerRef->GetInventorySize());
}

void UInventory::CombineItem(const int16 index)
{
	int16 inventoryRow;
	if (IsValid(PlayerRef))
	{
		inventoryRow = PlayerRef->GetInventoryRowSize();
	}
	else
	{
		return;
	}

	int16 dir[4] = { -inventoryRow, 1, inventoryRow, -1 };
	for (int16 i = 0; i < 4; i++)
	{
		int16 newdir = index + dir[i];
		RemoveItemAtIndex(newdir, 1);
	}
}

bool UInventory::SetLinkSlot(const int16 slot, const int16 newdir)
{
	const int16 combinableSlot = GetSlotWidgetInfo(newdir)->LinkedCombinableSlot;
	bool bactiveCombineBottun = false;

	if (GetSlotWidgetInfo(combinableSlot))
	{
		bactiveCombineBottun = GetSlotWidgetInfo(combinableSlot)->GetIsActiveCombineButton();
	}

	if ((combinableSlot == -1)/**the case wasn't linked before.*/
		|| (!bactiveCombineBottun && (combinableSlot != -1)) /**the case was linked before, but didn't successfully combine.*/
		||(slot == newdir))/**the case count 4*/
	{
		GetSlotWidgetInfo(newdir)->LinkedCombinableSlot = slot;
		return true;
	}

	return false;
}

bool UInventory::IsLineChange(const int16 slot)
{
	return (slot % InvetoryRow == 0) || ((slot + 1) % InvetoryRow == 0);
}

UCPP_Slot* UInventory::GetSlotWidgetInfo(const int16 index)
{
	if (index == -1)
	{
		return nullptr;
	}
	return InventoryWidget->SlotWidgetArray[index];
}

void UInventory::ChangeItemInfo(FName itemInfoID, const int16 index)
{
	CombineItem(index);

	const FItemInfoTable* itemInfo = ItemDataTable->FindRow<FItemInfoTable>(itemInfoID, TEXT(""));
	if (itemInfo == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("not valid ChangeItemInfo!! please set 'CombinResultID' from ItemDataTable!!"));
		return;
	}
	UItem* item = SlotsArray[index].Item;

	item->ItemInfoTable = *itemInfo;

	///**item data*/
	//item->ItemInfoTable.ItemInfoID = itemInfo->ItemInfoID;
	//item->ItemInfoTable.Name = itemInfo->Name;
	//item->ItemInfoTable.Description = itemInfo->Description;
	//item->ItemInfoTable.bCanBeUsed = itemInfo->bCanBeUsed;
	//item->ItemInfoTable.bCanStacked = itemInfo->bCanStacked;
	//item->ItemInfoTable.UseText = itemInfo->UseText;
	//item->ItemInfoTable.Interaction = itemInfo->Interaction;
	//item->ItemInfoTable.ItemPrice = itemInfo->ItemPrice;
	//item->ItemInfoTable.Weight = itemInfo->Weight;
	//item->ItemInfoTable.ATK = itemInfo->ATK;

	///**itemtype data*/
	//item->ItemType = itemInfo->ItemType;
	//item->ConsumeValue = itemInfo->ConsumeValue;
	//if (itemInfo->ItemClass)
	//{
	//	item->ItemClass = itemInfo->ItemClass;
	//}

	///**asset data*/
	//item->ItemMesh = itemInfo->ItemMesh;
	//item->IconTexture = itemInfo->IconTexture;

	UpdateSlotAtIndex(index);
}

void UInventory::InventorySort(int16 left, int16 right)
{
	if (left > right)
	{
		return;
	}

	int16 pivot = Partition(left, right);
	InventorySort(left, pivot - 1);
	InventorySort(pivot + 1, right);
}

int16 UInventory::Partition(int16 left, int16 right)
{
	int16 pivot = GetCompaireValue(left);
	int16 low = left + 1;
	int16 high = right;

	while (low <= high)
	{
		while ((low <= right) && (GetCompaireValue(low) <= pivot))
		{
			low++;
		}
		while ((high >= left + 1) && (GetCompaireValue(high) >= pivot))
		{
			high--;
		}

		if (low < high)
			SwapSlot(low, high);
	}

	SwapSlot(left, high);
	return high;
}

uint8 UInventory::GetCompaireValue(int16 index)
{
	if (IsSlotEmpty(index))
	{
		return (uint8)EItemCategory::EIS_None;
	}

	return (uint8)SlotsArray[index].Item->ItemInfoTable.ItemType;
}

void UInventory::SetEquipWeapon(UItem* item, const int16 index)
{
	UCPP_EquipSlot* equipSlot = InventoryWidget->EquipmentInventory->EquipSlot;

	/**swap Inventory item and EquipmentInventory item */
	if (IsValid(equipSlot))
	{
		UItem* equipRef = equipSlot->GetItemRef();


		/**set Inventory item to EquipmentInventory item*/
		if (IsValid(equipRef))
		{
			UpdateInventory(index, equipRef, 1);//item amount 1
		}
		else
		{
			UpdateInventory(index, equipRef, 0);//item amount 0
		}

		/**set EquipmentInventory item to Inventory item*/
		//UpdateEquipmentInventory(item);
		EquipWeaponToPlayer(item);
	}
	
}

void UInventory::EquipWeaponToPlayer(UItem* item)
{
	if(!IsValid(PlayerRef))
		return;
	
	PlayerRef->SetEquipWeapon(item);
}

void UInventory::UpdateEquipmentInventory(UItem* item)
{
	if(!IsValid(item))
		return;
	
	if(!IsValid(InventoryWidget))
		return;

	UCPP_EquipmentInventory* EquipmentInventory = InventoryWidget->EquipmentInventory;
	if(!IsValid(EquipmentInventory))
		return;

	EquipmentInventory->UpdateEquipSlot(item);
}

void UInventory::UnEquipWeaponAndAddItem(const int16 index)
{
	UCPP_EquipSlot* equipSlot = InventoryWidget->EquipmentInventory->EquipSlot;
	UItem* equipRef = equipSlot->GetItemRef();
	UpdateInventory(index, equipRef, 1);
	equipSlot->TakeOffWeapon();
}

AActor* UInventory::GetAbilityActor(FName itemId)
{
	AActor** abilityActor = ItemManageSystem.Find(itemId);
	if (abilityActor)
	{
		return *abilityActor;
	}
	return nullptr;
}

void UInventory::StartAbilityActorLife(FName itemId)
{
	AActor** abilityActor = ItemManageSystem.Find(itemId);
	if (abilityActor)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("DestroyAbilityActor"),*abilityActor, itemId);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 5.f, false);
	}
	
}

void UInventory::DestroyAbilityActor(AActor* actor, FName itemId)
{
	if (actor)
	{
		actor->Destroy();
		ItemManageSystem[itemId] = nullptr;

		FString id = itemId.ToString();
		GEngine->AddOnScreenDebugMessage(
			INDEX_NONE,
			30.f,
			FColor::Yellow,
			FString::Printf(TEXT("Destroy 'ID:%s ' from ItemManageSystem!"), *id));
	}
}

