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
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* SlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextAmount;

	
public: 
	
	void UpdateSlot(int32 index);
	FORCEINLINE void SetInventoryWidget(class UCPP_InventoryWidget* inventoryWidget) { InventoryWidget = inventoryWidget; }

private:

	UPROPERTY()
		class UCPP_InventoryWidget* InventoryWidget;

};
