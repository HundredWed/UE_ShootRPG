#include "Inventory.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Widget/CPP_Slot.h"
#include "Widget/CPP_EquipmentInventory.h"
#include "Widget/CPP_EquipSlot.h"
#include "Item/PickUpItem.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Controller.h"
#include "DataAssets/CPP_ConsumptionItemDataAsset.h"
#include "Systems/CPP_AkashicSubsystem.h"

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
		IsConnected.SetNum(PlayerRef->GetInventorySize());
		ClearConnectArray();

		InventoryRow = PlayerRef->GetInventoryRowSize();
		MaxWeight = PlayerRef->GetPlayerWeightInfo();


		//임시
		ACPP_Controller* controller = Cast<ACPP_Controller>(PlayerRef->GetController());
		InventoryWidget = controller->GetInventoryWidget();
		InventoryWidget->UpdateWeightMaxAmount(MaxWeight);
		InventoryWidget->UpdateWeightText(0);
		InventoryWidget->GenerateSlotWidget(InventoryRow);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not found PlayerRef at Inventory!!"));
	}
}

bool UInventory::IsSlotEmpty(const int32 index)
{
	UItem* item = SlotsArray[index].Item;

	if (item == nullptr)
	{
		return true;
	}

	return false;
	
}

int32 UInventory::AddItem(UItem* item, const uint32 amount)
{
	if (!IsValid(item) || amount == 0)
	{
		return 0;
	}

	uint32 remainAmount = amount;

	if (item->ItemInfoTable.bCanStacked)
	{
		while (remainAmount > 0)
		{
			int32 stackSlotIndex;
			if (!SearchFreeStackSlot(item, stackSlotIndex))
			{
				break;
			}

			uint32 addableAmount = MaxStackSize - SlotsArray[stackSlotIndex].ItemAmount;
			uint32 amountToPut = FMath::Min(remainAmount, addableAmount);
			UItem* itemToPut = SlotsArray[stackSlotIndex].Item;

			UpdateInventory(stackSlotIndex, itemToPut, SlotsArray[stackSlotIndex].ItemAmount + amountToPut);
			AddWeight(itemToPut->ItemInfoTable.Weight * amountToPut);

			remainAmount -= amountToPut;
		}

		while (remainAmount > 0)
		{
			int32 emptySlotIndex;
			
			if (!SearchEmptySlot(emptySlotIndex))
			{
				break;
			}

			uint32 amountToPut = FMath::Min(remainAmount, (uint32)MaxStackSize);
			UItem* itemToPut = item->CreateItemCopy(this);

			UpdateInventory(emptySlotIndex, itemToPut, amountToPut);
			AddWeight(itemToPut->ItemInfoTable.Weight * amountToPut);

			remainAmount -= amountToPut;
		}
	}
	else /** 스택 불가능한 아이템 (장비 등) */
	{
		while (remainAmount > 0)
		{
			int32 emptySlotIndex;

			if (!SearchEmptySlot(emptySlotIndex))
			{
				break;
			}

			UItem* itemToPut = item->CreateItemCopy(this);

			UpdateInventory(emptySlotIndex, itemToPut, 1);
			AddWeight(itemToPut->ItemInfoTable.Weight * 1);

			remainAmount -= 1;
		}
	}

	const int32 storedAmount = amount - remainAmount;

	return storedAmount;
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

	/**not enough inventory slot!! */
	return false;
}

bool UInventory::SearchFreeStackSlot(class UItem* item, int32& canStackedSlotIndex)
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

	/**not enough inventory slot!! */
	return false;
}

int32 UInventory::GetAmountAtIndex(const int32 index)
{
	return SlotsArray[index].ItemAmount;
}


bool UInventory::RemoveItemAtIndex(const int32 index, const int32 removeAmount)
{
	if (!IsSlotEmpty(index) && (removeAmount > 0))
	{
		if (SlotsArray[index].Item->ItemInfoTable.ItemType == EItemCategory::EIS_QuestItems)
		{
			return false;
		}

		const int32 amount = GetAmountAtIndex(index);
		const float weight = SlotsArray[index].Item->ItemInfoTable.Weight;
		const FName itemId = SlotsArray[index].Item->ItemInfoTable.ItemInfoID;

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

		OnItemRemoved.Execute(itemId, -removeAmount);
	}

	return true;
}

void UInventory::RemoveQuestItem(const FName& itemId, const int32 amount)
{
	int32 totalAmount = amount;

	for (int32 i = 0; i < SlotsArray.Num(); ++i)
	{
		const FInventorySlot& slot = SlotsArray[i];

		if (IsValid(slot.Item) && slot.Item->ItemInfoTable.ItemInfoID == itemId)
		{
			int32 removeAmount = FMath::Min(totalAmount, slot.ItemAmount);

			RemoveItemAtIndex(i, removeAmount);
			totalAmount -= removeAmount;

			if (totalAmount == 0)
			{
				break;
			}
		}
	}
}

void UInventory::SwapSlot(const int32 fromIndex, const int32 toIndex)
{
	CheckItemType(fromIndex, toIndex);
	CheckItemType(toIndex, fromIndex);
		
	const int32 lastSlot = SlotsArray.Num() - 1;

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

void UInventory::CheckItemType(const int32 fromIndex, const int32 toIndex)
{
	if (SlotsArray[toIndex].Item 
		&& SlotsArray[toIndex].Item->ItemInfoTable.ItemType == EItemCategory::EIS_Combinables)
	{
		InventoryWidget->SlotWidgetArray[toIndex]->CheckCombinability(fromIndex);
	}
}

void UInventory::AddToIndex(const int32 fromIndex, const int32 toIndex)
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

bool UInventory::CanAddToIndex(const int32 fromIndex, const int32 toIndex)
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

void UInventory::UpdateInventory(int32 index, UItem* item, int32 amount)
{
	SlotsArray[index].Item = item;
	SlotsArray[index].ItemAmount = amount;

	UpdateSlotAtIndex(index);
}


void UInventory::SplitStackToIndex(const int32 fromIndex, const int32 toIndex, const int32 splitAmount)
{
	if (CanSplitStackable(fromIndex, toIndex, splitAmount))
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

bool UInventory::CanSplitStackable(const int32 fromIndex, const int32 toIndex, const int32 splitAmount)
{
	return IsSlotEmpty(toIndex)
		&& (SlotsArray[fromIndex].Item->ItemInfoTable.bCanStacked)
		&& (SlotsArray[fromIndex].ItemAmount > 1) 
		&& (SlotsArray[fromIndex].ItemAmount > splitAmount);
}

void UInventory::UpdateSlotAtIndex(const int32 index)
{
	InventoryWidget->SlotWidgetArray[index]->UpdateSlot(index);
}

const FInventorySlot UInventory::GetSlotInfoIndex(const int32 index)
{
	return SlotsArray[index];
}

void UInventory::AddWeight(const float amount)
{
	CurrentWeight += amount;
	InventoryWidget->UpdateWeightText(CurrentWeight);
}

void UInventory::AddGold(const int32 amount)
{
	CurrentGold += amount;
	InventoryWidget->UpdateGoldText(CurrentGold);
}

int32 UInventory::FindCombinableSlot(const int32 slot)
{
	IsConnected[slot] = true;
	int32 count = 0;
	int32 resultSlot = 0;
	int32 dir[4] = { -InventoryRow, 1, InventoryRow, -1 };

	if (IsLineChange(slot) == false)
	{
		for (int32 i = 0; i < 4; i++)
		{
			int32 newdir = slot + dir[i];
			if ((newdir < 0) || (newdir > (IsConnected.Num() - 1)) || !CompareID(slot, newdir))
			{
				continue;
			}

			if (CompareID(slot, newdir))
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

	for (int32 i = 0; i < 4; i++)
	{
		int32 newdir = slot + dir[i];
		
		if ((newdir < 0) || (newdir > (IsConnected.Num() - 1)) || !CompareID(slot, newdir) || (IsConnected[newdir] == true))
		{
			continue;
		}

		if (CompareID(slot, newdir) && (IsConnected[newdir] == false))
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

bool UInventory::CompareID(const int32 slot1, const int32 slot2)
{
	if (SlotsArray[slot2].Item == nullptr)
	{
		return false;
	}

	return SlotsArray[slot1].Item->ItemInfoTable.ItemInfoID == SlotsArray[slot2].Item->ItemInfoTable.ItemInfoID;
}

void UInventory::ClearConnectArray()
{
	IsConnected.Init(false, PlayerRef->GetInventorySize());
}

void UInventory::CombineItem(const int32 index)
{
	int32 inventoryRow;
	if (IsValid(PlayerRef))
	{
		inventoryRow = PlayerRef->GetInventoryRowSize();
	}
	else
	{
		return;
	}

	int32 dir[4] = { -inventoryRow, 1, inventoryRow, -1 };
	for (int32 i = 0; i < 4; i++)
	{
		int32 newdir = index + dir[i];
		RemoveItemAtIndex(newdir, 1);
	}
}

bool UInventory::SetLinkSlot(const int32 slot, const int32 newdir)
{
	const int32 combinableSlot = GetSlotWidgetInfo(newdir)->LinkedCombinableSlot;
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

bool UInventory::IsLineChange(const int32 slot)
{
	return (slot % InventoryRow == 0) || ((slot + 1) % InventoryRow == 0);
}

UCPP_Slot* UInventory::GetSlotWidgetInfo(const int32 index)
{
	if (index == -1)
	{
		return nullptr;
	}
	return InventoryWidget->SlotWidgetArray[index];
}

void UInventory::ChangeItemInfo(FName itemInfoID, const int32 index)
{
	CombineItem(index);

	const FItemInfoTable* itemInfo = ItemDataTable->FindRow<FItemInfoTable>(itemInfoID, TEXT(""));
	if (itemInfo == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("not valid ChangeItemInfo!! please set 'CombineResultID' from ItemDataTable!!"));
		return;
	}
	UItem* item = SlotsArray[index].Item;

	item->ItemInfoTable = *itemInfo;

	UpdateSlotAtIndex(index);
}

void UInventory::InventorySort(int32 left, int32 right)
{
	if (left > right)
	{
		return;
	}

	int32 pivot = Partition(left, right);
	InventorySort(left, pivot - 1);
	InventorySort(pivot + 1, right);
}

int32 UInventory::Partition(int32 left, int32 right)
{
	int32 pivot = GetCompareValue(left);
	int32 low = left + 1;
	int32 high = right;

	while (low <= high)
	{
		while ((low <= right) && (GetCompareValue(low) <= pivot))
		{
			low++;
		}
		while ((high >= left + 1) && (GetCompareValue(high) >= pivot))
		{
			high--;
		}

		if (low < high)
			SwapSlot(low, high);
	}

	SwapSlot(left, high);
	return high;
}

uint8 UInventory::GetCompareValue(int32 index)
{
	if (IsSlotEmpty(index))
	{
		return (uint8)EItemCategory::EIS_None;
	}

	return (uint8)SlotsArray[index].Item->ItemInfoTable.ItemType;
}

void UInventory::SetEquipWeapon(UItem* item, const int32 index)
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
		UpdateEquipmentInventory(item);
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

void UInventory::UnEquipWeaponAndAddItem(const int32 index)
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

int32 UInventory::GetTotalItemAmount(const FName& itemID)
{
	int32 amount = 0;

	for (FInventorySlot slot : SlotsArray)
	{
		if (slot.Item == nullptr)
		{
			continue;
		}

		if (slot.Item->ItemInfoTable.ItemInfoID == itemID)
		{
			amount += slot.ItemAmount;			
		}
	}

	return amount;
}

void UInventory::UseItem(const int32 index)
{
	if (UCPP_ConsumptionItemDataAsset* asset = SlotsArray[index].Item->ItemInfoTable.ItemLogicAsset)
	{
		asset->ExecuteLogic(GetOwner());
	}
}

void UInventory::ExchangeQuestItems(const FName& rewardItemID, const int32 rewardAmount, const FName& removeItemID, const int32 removeAmount)
{
	//먼저 삭제
	RemoveQuestItem(removeItemID, removeAmount);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (UCPP_AkashicSubsystem* AS = World->GetSubsystem<UCPP_AkashicSubsystem>())
	{
		const FItemInfoTable* itemInfo = AS->RequestItemInfo(rewardItemID);
		if (itemInfo)
		{
			UItem* newItem = NewObject<UItem>(UItem::StaticClass());
			newItem->ItemInfoTable = *itemInfo;
			AddItem(newItem, rewardAmount);
		}
	}

}

