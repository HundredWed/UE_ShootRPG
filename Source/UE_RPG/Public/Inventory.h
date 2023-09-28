// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
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

	/**inventory value*/
	float MaxWeight = 0.0f;
	int32 MaxGold = 999999999;
protected:
	
	virtual void BeginPlay() override;
	
public:

	/**inventory function*/
	bool IsSlotEmpty(const int16 index);
	void AddItem(class UItem* item, const uint32 amount = 1);
	bool SearchEmptySlot(int16& emptySlotIndex);
	bool SearchFreeStackSlot(class UItem* item, int16& canStackedSlotIndex);
	int32 GetAmountAtIndex(const int16 index);

	void RemoveItemAtIndex(const int16 index, const int32 removeAmount);
	void SwapSlot(const int16 fromIndex, const int16 toIndex);
	void CheckItemType(const int16 fromIndex, const int16 toIndex);
	void AddToIndex(const int16 fromIndex, const int16 toIndex);
	bool CanAddToIndex(const int16 fromIndex, const int16 toIndex);
	void UpdateInventory(int16 index, class UItem* item, int32 amount);
	void UpdateSlotAtIndex(const int16 index);
	const FInventorySlot GetSlotInfoIndex(const int16 index);
	void AddWeight(const float amount);
	void AddGold(const int32 amount);
	const int32 GetCurrentGold() { return CurrentGold; }
	bool IsOverGold(const int32 amount) { return (amount + CurrentGold) > MaxGold; }

	/**split when drag slot*/
	void SplitStackToIndex(const int16 fromIndex, const int16 toIndex, const int32 splitAmount);
	bool CanSplitStakable(const int16 fromIndex, const int16 toIndex, const int32 splitAmount);

	/**FindCombinableSlot function*/
	int16 FindCombinableSlot(const int16 slot);
	bool CompaireID(const int16 slot1, const int16 slot2);
    void ClearConectArray();
	void CombineItem(const int16 index);
	bool SetLinkSlot(const int16 slot, const int16 newdir);
	bool IsLineChange(const int16 slot);
	class UCPP_Slot* GetSlotWidgetInfo(const int16 index);
	void ChangeItemInfo(FName itemInfoID, const int16 index);
	
	void InventorySort(int16 left, int16 right);
	int16 Partition(int16 left, int16 right);
	uint8 GetCompaireValue(int16 index);

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
	void SetEquipWeapon(class UItem* item, int16 index);
	void EquipWeaponToPlayer(class UItem* item);
	void UpdateEquipmentInventory(class UItem* item);
	void UnEquipWeaponAndAddItem(const int16 index);

	/**manage ability-actor*/
	AActor* GetAbilityActor(FName itemId);
	void StartAbilityActorLife(FName itemId);

	UFUNCTION()
		void DestroyAbilityActor(AActor* actor,FName itemId);

private:

	UPROPERTY()
		class ACPP_Character* PlayerRef;

	/**inventory value*/
	uint8 MaxStackSize = 99;
	uint8 InvetoryRow = 0;
	float CurrnetWeight = 0.0f;
	int32 CurrentGold = 0;
	


	TMap<FName, AActor*> ItemManageSystem;

	/**for FindCombinableSlot function*/
	TArray<bool> isConect;
};
