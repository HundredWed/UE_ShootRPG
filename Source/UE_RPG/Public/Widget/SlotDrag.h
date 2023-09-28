// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SlotDrag.generated.h"

/**
 * DragDropOperation class
 */
UCLASS()
class UE_RPG_API USlotDrag : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
		class UCPP_Slot* WidgetRef = nullptr;

	bool bFromEquipmentSlot = false;
};
