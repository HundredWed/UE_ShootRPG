// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/ST_Quest.h"
#include "Inventory.generated.h"

class ACPP_Character;
struct FEquipmentInfoTable;

struct FInventorySlot
{
	FName ItemID = NAME_None;
	int32 ItemAmount = 1;
};

DECLARE_DELEGATE_TwoParams(FOnOnItemRemovedDelegate, const FName&, const int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventory();

	/**widget value*/
	UPROPERTY()
		UCPP_InventoryWidget* InventoryWidget;
	
	FOnOnItemRemovedDelegate OnItemRemoved;

protected:
	
	virtual void BeginPlay() override;
	
public:

	/**inventory function*/
	bool IsSlotEmpty(const int32 index);
	int32 AddItem(const FName& itemID, const int32 amount = 1);
	bool SearchEmptySlot(int32& emptySlotIndex);
	bool SearchFreeStackSlot(const FName& itemID, int32& canStackedSlotIndex);
	int32 GetAmountAtIndex(const int32 index);

	bool RemoveItemAtIndex(const int32 index, const int32 removeAmount);
	void SwapSlot(const int32 fromIndex, const int32 toIndex);
	void CheckItemType(const int32 fromIndex, const int32 toIndex);
	void AddToIndex(const int32 fromIndex, const int32 toIndex);
	bool CanAddToIndex(const int32 fromIndex, const int32 toIndex);
	void UpdateInventory(int32 index, const FName& itemID, const int32 amount = 1);
	void UpdateSlotAtIndex(const int32 index);
	const FInventorySlot GetSlotInfoIndex(const int32 index);
	void AddWeight(const float amount);
	int32 AddGold(const int32 amount);
	const int32 GetCurrentGold() { return CurrentGold; }
	bool IsOverGold(const int32 amount) { return (amount + CurrentGold) > MaxGold; }

	/**split when drag slot*/
	void SplitStackToIndex(const int32 fromIndex, const int32 toIndex, const int32 splitAmount);
	bool CanSplitStackable(const int32 fromIndex, const int32 toIndex, const int32 splitAmount);

	/**FindCombinableSlot function*/
	int32 FindCombinableSlot(const int32 slot);
	bool CompareID(const int32 slot1, const int32 slot2);
    void ClearConnectArray();
	void CombineItem(const int32 index);
	bool SetLinkSlot(const int32 slot, const int32 newdir);
	bool IsLineChange(const int32 slot);
	class UCPP_Slot* GetSlotWidgetInfo(const int32 index);
	void ChangeItemInfo(const FName& itemID, const int32 index);
	
	void InventorySort(int32 left, int32 right);
	int32 Partition(int32 left, int32 right);
	uint8 GetCompareValue(int32 index);

	/**inventory widget function (FORCEINLINE) */
	void ShowInventory(); 
	void HideInventory();

	/**equip weapon*/
	/**this func set equipment and swap weapon*/
	void SetEquipWeapon(const FName& itemID, int32 index);
	void EquipWeaponToPlayer(const FName& itemID);
	void UpdateEquipmentInventory(const FName& itemID);
	void UnEquipWeaponAndAddItem(const int32 index);

	/**getter*/
	int32 GetTotalItemAmount(const FName& itemID);
	int32 GetInventorySize();

	void UseItem(const int32 index);

	void ExchangeQuestItems(const FName& rewardItemID, const int32 rewardAmount, const FName& removeItemID, const int32 removeAmount);
    
	FItemInfoTable* RequestItemData(const FName& itemId);
	FEquipmentInfoTable* RequestEquipmentData(const FName& itemId);

private:	

	void RemoveQuestItem(const FName& itemId, const int32 amount);

	
private:

	UPROPERTY()
	ACPP_Character* PlayerRef;

	/**inventory slot*/
	UPROPERTY()
	TArray<FInventorySlot> SlotsArray;

	/**inventory value*/
	int32 MaxStackSize = 99;
	int32 InventoryRow = 0;
	float CurrentWeight = 0.0f;
	int32 CurrentGold = 0;
	float MaxWeight = 0.0f;
	int32 MaxGold = 99999999999;

	/**for FindCombinableSlot function*/
	TArray<bool> IsConnected;
};
