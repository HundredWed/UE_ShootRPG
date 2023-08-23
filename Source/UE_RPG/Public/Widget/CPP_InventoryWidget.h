// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_InventoryWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* CloseButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UUniformGridPanel* SlotPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
		TSubclassOf< class UCPP_Slot> SlotWidgetClass;

	UPROPERTY()
		TArray<class UCPP_Slot*> SlotWidgetArray;

	

	UFUNCTION()
		void GenerateSlotWidget(int32 slotsParRow);

private:

	UPROPERTY()
		class UCPP_Slot* SlotWidget;

};
