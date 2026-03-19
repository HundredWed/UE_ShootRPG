// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Structs/ST_Quest.h"
#include "Widget/CPP_InventoryWidget.h"
#include "Inventory.generated.h"


USTRUCT(BlueprintType)
struct FInventorySlot : public FTableRowBase
{
	GENERATED_BODY()

		FInventorySlot() :Item(nullptr), ItemAmount(0) {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UItem* Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemAmount;
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

	/**inventory slot*/
	UPROPERTY()
		TArray<FInventorySlot> SlotsArray;

	UPROPERTY(EditAnywhere, Category = "Item state")
		UDataTable* ItemDataTable;

	
	FOnOnItemRemovedDelegate OnItemRemoved;

protected:
	
	virtual void BeginPlay() override;
	
public:

	/**inventory function*/
	bool IsSlotEmpty(const int32 index);
	int32 AddItem(class UItem* item, const uint32 amount = 1);
	bool SearchEmptySlot(int32& emptySlotIndex);
	bool SearchFreeStackSlot(class UItem* item, int32& canStackedSlotIndex);
	int32 GetAmountAtIndex(const int32 index);

	bool RemoveItemAtIndex(const int32 index, const int32 removeAmount);
	void SwapSlot(const int32 fromIndex, const int32 toIndex);
	void CheckItemType(const int32 fromIndex, const int32 toIndex);
	void AddToIndex(const int32 fromIndex, const int32 toIndex);
	bool CanAddToIndex(const int32 fromIndex, const int32 toIndex);
	void UpdateInventory(int32 index, class UItem* item, int32 amount);
	void UpdateSlotAtIndex(const int32 index);
	const FInventorySlot GetSlotInfoIndex(const int32 index);
	void AddWeight(const float amount);
	void AddGold(const int32 amount);
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
	void ChangeItemInfo(FName itemInfoID, const int32 index);
	
	void InventorySort(int32 left, int32 right);
	int32 Partition(int32 left, int32 right);
	uint8 GetCompareValue(int32 index);

	/**inventory widget function (FORCEINLINE) */
	FORCEINLINE void ShowInventory() {
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetIsEnabled(true);
	}
	FORCEINLINE void HideInventory() {
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->SetIsEnabled(false);
	}

	FORCEINLINE void AddItemManage(FName itemId, AActor* iteAbilityClass) {
		ItemManageSystem.Add(itemId, iteAbilityClass);
	}

	/**equip weapon*/
	/**this func set equipment and swap weapon*/
	void SetEquipWeapon(class UItem* item, int32 index);
	void EquipWeaponToPlayer(class UItem* item);
	void UpdateEquipmentInventory(class UItem* item);
	void UnEquipWeaponAndAddItem(const int32 index);

	/**manage ability-actor*/
	AActor* GetAbilityActor(FName itemId);

	/**getter*/
	int32  GetTotalItemAmount(const FName& itemID);

	void UseItem(const int32 index);

	void ExchangeQuestItems(const FName& rewardItemID, const int32 rewardAmount, const FName& removeItemID, const int32 removeAmount);

private:	

	void RemoveQuestItem(const FName& itemId, const int32 amount);

private:

	UPROPERTY()
		class ACPP_Character* PlayerRef;

	/**inventory value*/
	uint8 MaxStackSize = 99;
	int16 InventoryRow = 0;
	float CurrentWeight = 0.0f;
	int32 CurrentGold = 0;
	float MaxWeight = 0.0f;
	int32 MaxGold = 999999999;

	UPROPERTY()
		TMap<FName, AActor*> ItemManageSystem;

	/**for FindCombinableSlot function*/
	TArray<bool> IsConnected;
};
