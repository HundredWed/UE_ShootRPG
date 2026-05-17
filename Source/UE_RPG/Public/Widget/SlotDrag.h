// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item/Weapon/EquipmentData.h"
#include "SlotDrag.generated.h"

UENUM(BlueprintType)
enum class ESlotSourceType : uint8
{
	Inventory,
	Equipment,
	QuickSlot
};

UCLASS()
class UE_RPG_API USlotDrag : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	ESlotSourceType SourceType;
	FName SourceID; 
	int32 SlotIndex;
	int32 CachedTypeFlag = -1;
};
