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
protected:
	
	virtual void BeginPlay() override;
	
public:

	/**inventory function*/
	bool IsSlotEmpty(const uint8 index);
	void AddItem(class UItem* item, const uint32 amount);
	bool SearchEmptySlot(uint8& emptySlotIndex);
	bool SearchFreeStackSlot(class UItem* item, uint8& canStackedSlotIndex);
	int32 GetAmountAtIndex(const uint8 index);

	void RemoveItemAtIndex(const uint8 index, const uint32 removeAmount);
	void SwapSlot(const uint8 fromIndex, const uint8 toIndex);
	void AddToIndex(const uint8 fromIndex, const uint8 toIndex);
	bool CanAddToIndex(const uint8 fromIndex, const uint8 toIndex);
	
	/**split when drag slot*/
	void SplitStackToIndex(const uint8 fromIndex, const uint8 toIndex, const int32 splitAmount);
	bool CanSplitStakable(const uint8 fromIndex, const uint8 toIndex, const int32 splitAmount);

	void UpdateSlotAtIndex(const uint8 index);
	const FInventorySlot GetSlotInfoIndex(const uint8 index);

	/**FindCombinableSlot function*/
	int8 FindCombinableSlot(const int8 slot);
	bool CompaireID(const uint8 slot1, const uint8 slot2);
    void ClearConectArray();
	void CombineItem(const uint8 index);
	
	

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

	TMap<FName, AActor*> ItemManageSystem;

	/**for FindCombinableSlot function*/
	TArray<bool> isConect;
};
