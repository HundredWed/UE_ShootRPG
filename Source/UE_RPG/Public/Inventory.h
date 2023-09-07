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

	/**main panel widget*/
	UPROPERTY(EditAnywhere, Category = "Main Widget")
		TSubclassOf< class UMainPanelWidget> MainPanelclass;

	/**inventory slot*/
	UPROPERTY()
		TArray<FInventorySlot> SlotsArray;
protected:
	
	virtual void BeginPlay() override;
	
public:

	/**inventory function*/
	bool IsSlotEmpty(const uint8 index);
	void AddItem(class UItem* item, const uint8 amount);
	bool SearchEmptySlot(uint8& emptySlotIndex);
	bool SearchFreeStackSlot(class UItem* item, uint8& canStackedSlotIndex);
	int32 GetAmountAtIndex(const uint8 index);
	void RemoveItemAtIndex(const uint8 index, const uint8 removeAmount);


	UFUNCTION()
		FInventorySlot GetSlotInfoIndex(const uint8 index);

	/**inventory widget function (FORCEINLINE) */
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
	uint8 MaxStackSize = 99;
	
	/**widget value*/
	UCPP_InventoryWidget* InventoryWidget;

	UPROPERTY()
		class ACPP_Character* PlayerRef;

	UPROPERTY()
		class UMainPanelWidget* MainPanelWidget;
};
