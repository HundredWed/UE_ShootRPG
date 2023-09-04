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
	bool IsSlotEmpty(int32 index);
	void AddItem(class UItem* item, int32 amount);
	bool SearchEmptySlot(int32& emptySlotIndex);
	bool SearchFreeStackSlot(class UItem* item, int32& canStackedSlotIndex);
	int32 GetAmountAtIndex(int32 index);
	void RemoveItemAtIndex(const int32 index, const int32 removeAmount);


	UFUNCTION()
		FInventorySlot GetSlotInfoIndex(const int32 index);

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
	int32 MaxStackSize = 99;
	
	/**widget value*/
	UCPP_InventoryWidget* InventoryWidget;

	UPROPERTY()
		class ACPP_Character* PlayerRef;

	UPROPERTY()
		class UMainPanelWidget* MainPanelWidget;
};
