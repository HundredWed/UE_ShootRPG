// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_EquipmentInventory.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_EquipmentInventory : public UCustomUMGWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCPP_EquipSlot* EquipSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ATKText;

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

public:
	void UpdateEquipSlot(class UItem* item);
};
