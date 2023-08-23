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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AItem* Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemAmount;

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventory();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	
	virtual void BeginPlay() override;

public:	
	
	FORCEINLINE TArray<class AItem*> GetInventory() { return Inventory; }

	FInventorySlot Slot;


	/**inventory function*/
	bool IsSlotEmpty(int32 index);
	void AddItem(class AItem* item, int32 amount);
	bool SearchEmptySlot(int32& emptySlotIndex);
	bool SearchFreeStackSlot(class AItem* item, int32& canStackedSlotIndex);
	void GetItemInfoIndex(const int32 index, class AItem& item, int32& amount);

	/**inventory widget function*/
	FORCEINLINE void ShowInventory() {
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetIsEnabled(true);
	}
	FORCEINLINE void HideInventory() {
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->SetIsEnabled(false);
	}

private:

	/**inventory value*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class AItem*> Inventory;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<FInventorySlot> SlotsArray;

	int32 MaxStackSize = 99;
	
	/**inventory widget value*/
	UCPP_InventoryWidget* InventoryWidget;
	

};
