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

	UCPP_Slot();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UInventory* _Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 _SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 _Amount;

public: 
	
	void UpdateSlot();
};
