#include "Inventory.h"
#include "CPP_Character.h"
#include "Widget/CPP_Slot.h"
#include "Widget/CPP_EquipSlot.h"
#include "Item/PickUpItem.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Controller.h"
#include "DataAssets/CPP_ConsumptionItemDataAsset.h"
#include "Widget/CPP_InventoryWidget.h"
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
	const FName itemId = SlotsArray[index].ItemID;

	return itemId.IsNone();	
}

int32 UInventory::AddItem(const FName& itemID, const int32 amount)
{
	FItemInfoTable* itemData = RequestItemData(itemID);

	if (itemData == nullptr || amount == 0)
	{
		return 0;
	}

	if (itemData->ItemType == EItemCategory::EIC_Gold)
	{
		return AddGold(amount);
	}

	int32 remainAmount = amount;

	if (itemData->bCanStacked)
	{
		while (remainAmount > 0)
		{
			int32 stackSlotIndex;
			if (!SearchFreeStackSlot(itemID, stackSlotIndex))
			{
				break;
			}

			int32 addableAmount = MaxStackSize - SlotsArray[stackSlotIndex].ItemAmount;
			int32 amountToPut = FMath::Min(remainAmount, addableAmount);
			int32 newAmount = SlotsArray[stackSlotIndex].ItemAmount + amountToPut;

			UpdateInventory(stackSlotIndex, itemID, newAmount);
			AddWeight(itemData->Weight * amountToPut);

			remainAmount -= amountToPut;
		}

		while (remainAmount > 0)
		{
			int32 emptySlotIndex;
			
			if (!SearchEmptySlot(emptySlotIndex))
			{
				break;
			}

			int32 amountToPut = FMath::Min(remainAmount, MaxStackSize);

			UpdateInventory(emptySlotIndex, itemID, amountToPut);
			AddWeight(itemData->Weight * amountToPut);

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

			UpdateInventory(emptySlotIndex, itemID, 1);
			AddWeight(itemData->Weight * 1);

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
		if (SlotsArray[index].ItemID.IsNone())
		{
			emptySlotIndex = index;
			return true;
		}
	}

	/**not enough inventory slot!! */
	return false;
}

bool UInventory::SearchFreeStackSlot(const FName& itemId, int32& canStackedSlotIndex)
{
	for (int32 index = 0; index < SlotsArray.Num(); index++)
	{
		const FName slotItemID = SlotsArray[index].ItemID;
		const int32 slotItemAmount = SlotsArray[index].ItemAmount;
		if (slotItemID.IsNone())
		{
			if ((slotItemID == itemId) && (slotItemAmount < MaxStackSize))
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
		const FItemInfoTable* itemData = RequestItemData(SlotsArray[index].ItemID);

		if (itemData->ItemType == EItemCategory::EIC_QuestItems)
		{
			return false;
		}

		const int32 amount = GetAmountAtIndex(index);
		const float weight = itemData->Weight;

		if (removeAmount >= amount)
		{ 
			UpdateInventory(index, NAME_None, 0);
		}
		else
		{
			SlotsArray[index].ItemAmount = amount - removeAmount;
			UpdateSlotAtIndex(index);
		}

		AddWeight(-(weight * removeAmount));

		OnItemRemoved.Execute(SlotsArray[index].ItemID, -removeAmount);
	}

	return true;
}

void UInventory::RemoveQuestItem(const FName& itemId, const int32 amount)
{
	int32 totalAmount = amount;

	for (int32 i = 0; i < SlotsArray.Num(); ++i)
	{
		if (SlotsArray[i].ItemID == itemId)
		{
			int32 removeAmount = FMath::Min(totalAmount, SlotsArray[i].ItemAmount);

			RemoveItemAtIndex(i, removeAmount);
			totalAmount -= removeAmount;

			if (totalAmount == 0)
			{
				break;
			}
		}
	}
}

void UInventory::RequestTakeOffWeapon()
{
	if (IsValid(PlayerRef))
	{
		PlayerRef->TakeOffWeapon();
	}
}

void UInventory::UpdateEquipSlot(const FItemInfoTable* itemData, const FEquipmentInfoTable* equipmentData)
{
	if (FEquipmentSlot* slot = EquipmentSlots.Find(equipmentData->EquipmentType))
	{
		slot->EquipmentID = equipmentData->EquipmentID;

	}
	else
	{
		FEquipmentSlot newSlot;
		newSlot.EquipmentID = equipmentData->EquipmentID;
		EquipmentSlots.Add(equipmentData->EquipmentType, newSlot);
	}

	InventoryWidget->UpdateEquipmentInventory(itemData, equipmentData);
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
	FItemInfoTable* toIndexItemData = RequestItemData(SlotsArray[toIndex].ItemID);

	if (toIndexItemData && toIndexItemData->ItemType == EItemCategory::EIC_Combinables)
	{
		InventoryWidget->CheckCombinability(toIndex, fromIndex);
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
			UpdateInventory(fromIndex, NAME_None, 0);
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
}

bool UInventory::CanAddToIndex(const int32 fromIndex, const int32 toIndex)
{
	if (IsSlotEmpty(toIndex))
	{
		return false;
	}

	FName fromIndexItemId = SlotsArray[fromIndex].ItemID;
	FName toIndexItemId = SlotsArray[toIndex].ItemID;

	FItemInfoTable* toIndexItemData = RequestItemData(toIndexItemId);

	bool brestAmount = SlotsArray[toIndex].ItemAmount < MaxStackSize;
	bool bstacked = toIndexItemData->bCanStacked;

	return (fromIndexItemId == toIndexItemId) && brestAmount && bstacked;
}

void UInventory::UpdateInventory(int32 index, const FName& itemID, const int32 amount)
{
	SlotsArray[index].ItemID = itemID;
	SlotsArray[index].ItemAmount = amount;

	UpdateSlotAtIndex(index);
}

void UInventory::SplitStackToIndex(const int32 fromIndex, const int32 toIndex, const int32 splitAmount)
{
	if (CanSplitStackable(fromIndex, toIndex, splitAmount))
	{
		SlotsArray[fromIndex].ItemAmount -= splitAmount;

		UpdateSlotAtIndex(fromIndex);
		UpdateInventory(toIndex, SlotsArray[fromIndex].ItemID);
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
		&& (SlotsArray[fromIndex].ItemAmount > 1) 
		&& (SlotsArray[fromIndex].ItemAmount > splitAmount);
}

void UInventory::UpdateSlotAtIndex(const int32 index)
{
	const FItemInfoTable* toIndexItemData = RequestItemData(SlotsArray[index].ItemID);

	InventoryWidget->UpdateSlot(toIndexItemData, index, SlotsArray[index].ItemAmount);
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

int32 UInventory::AddGold(const int32 amount)
{
	const int32 newTotal = CurrentGold + amount;
	if (newTotal > MaxGold)
	{
		return newTotal - MaxGold;
	}
	else
	{
		CurrentGold = FMath::Max(newTotal, MaxGold);
		InventoryWidget->UpdateGoldText(CurrentGold);

		const int32 allStored = 0;
		return allStored;
	}	
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
	if (SlotsArray[slot2].ItemID.IsNone())
	{
		return false;
	}

	return SlotsArray[slot1].ItemID == SlotsArray[slot2].ItemID;
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

void UInventory::ChangeItemInfo(const FName& itemId, const int32 index)
{
	CombineItem(index);

	const FItemInfoTable* itemInfo = RequestItemData(itemId);
	if (itemInfo == nullptr)
	{
		return;
	}
	
	SlotsArray[index].ItemID = itemId;
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

	//SwapSlot(left, high);
	return high;
}

uint8 UInventory::GetCompareValue(int32 index)
{
	if (IsSlotEmpty(index))
	{
		return (uint8)EItemCategory::EIC_None;
	}

	const FItemInfoTable* itemData = RequestItemData(SlotsArray[index].ItemID);
	if (itemData == nullptr)
	{
		return (uint8)EItemCategory::EIC_None;
	}

	return (uint8)itemData->ItemType;
}

void UInventory::SetEquipWeapon(const int32 fromIndex)
{
	const FName inputEquipmentID = SlotsArray[fromIndex].ItemID;
	const FEquipmentInfoTable* inputEquipmentData = RequestEquipmentData(inputEquipmentID);

	if (inputEquipmentData == nullptr)
		return;

	const FName currentEquipmentID = InventoryWidget->GetCurrentEquipmentID(inputEquipmentData->EquipmentType);

	//장비칸에 장비가 있으면 스왑. 없으면 해당 index빈칸으로 초기화
	if (!currentEquipmentID.IsNone())
	{
		UpdateInventory(fromIndex, currentEquipmentID, 1);//item amount 1
	}
	else
	{
		UpdateInventory(fromIndex, NAME_None, 0);//item amount 0
	}

	/**set EquipmentInventory item to Inventory item*/
	const FItemInfoTable* itemData = RequestItemData(inputEquipmentID);
	UpdateEquipSlot(itemData, inputEquipmentData);
	EquipWeaponToPlayer(inputEquipmentID);
}

void UInventory::EquipWeaponToPlayer(const FName& itemId)
{
	if(!IsValid(PlayerRef))
		return;
	
	PlayerRef->SetEquipWeapon(itemId);
}

void UInventory::UpdateEquipmentInventory(const FName& itemID)
{
	const FItemInfoTable* itemData = RequestItemData(itemID);
	const FEquipmentInfoTable* equipmentData = RequestEquipmentData(itemID);

	if (itemData == nullptr || equipmentData == nullptr)
	{
		return;
	}

	UpdateEquipSlot(itemData, equipmentData);
}

void UInventory::UnEquipWeaponAndAddToIndex(EEquipmentType equipmentType, const int32 index)
{
	if (FEquipmentSlot* slot = EquipmentSlots.Find(equipmentType))
	{
		const FName currentEquipmentID = slot->EquipmentID;
		const FItemInfoTable* itemData = RequestItemData(currentEquipmentID);
		UpdateInventory(index, currentEquipmentID, 1);

		RequestTakeOffWeapon();
		slot->EquipmentID = NAME_None;
	}	
}

int32 UInventory::GetTotalItemAmount(const FName& itemID)
{
	int32 amount = 0;

	for (FInventorySlot slot : SlotsArray)
	{
		if (slot.ItemID.IsNone())
		{
			continue;
		}

		if (slot.ItemID == itemID)
		{
			amount += slot.ItemAmount;			
		}
	}

	return amount;
}

int32 UInventory::GetInventorySize()
{
	return SlotsArray.Num();
}

void UInventory::UseItem(const int32 index)
{
	const FItemInfoTable* itemData = RequestItemData(SlotsArray[index].ItemID);
	if (itemData == nullptr)
	{
		return;
	}

	if (UCPP_ConsumptionItemDataAsset* asset = itemData->ItemLogicAsset)
	{
		asset->ExecuteLogic(GetOwner());
	}
}

void UInventory::ExchangeQuestItems(const FName& rewardItemID, const int32 rewardAmount, const FName& removeItemID, const int32 removeAmount)
{
	//먼저 삭제
	RemoveQuestItem(removeItemID, removeAmount);
	AddItem(rewardItemID, rewardAmount);
}

FItemInfoTable* UInventory::RequestItemData(const FName& itemId)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}

	if (UCPP_AkashicSubsystem* AS = World->GetSubsystem<UCPP_AkashicSubsystem>())
	{
		return AS->RequestItemInfo(itemId);
	}
	
	return nullptr;
}

FEquipmentInfoTable* UInventory::RequestEquipmentData(const FName& itemId)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return nullptr;
	}

	if (UCPP_AkashicSubsystem* AS = World->GetSubsystem<UCPP_AkashicSubsystem>())
	{
		return AS->RequestWeaponInfo(itemId);
	}

	return nullptr;
}

void UInventory::ShowInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	InventoryWidget->SetIsEnabled(true);
}

void UInventory::HideInventory()
{
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	InventoryWidget->SetIsEnabled(false);	
}
