// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_Slot.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_Slot : public UCustomUMGWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextAmount;


	FORCEINLINE void SetInventory(class UInventory* inventory) { Inventory = inventory; }
	FORCEINLINE void SetInventoryWidget(class UCPP_InventoryWidget* inventoryWidget) { InventoryWidget = inventoryWidget; }
public: 
	
	void UpdateSlot(int32 index);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess = "true"))
		class UInventory* Inventory;

	UPROPERTY()
		class UCPP_InventoryWidget* InventoryWidget;

};
