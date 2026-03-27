// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item/Weapon/EquipmentData.h"
#include "SlotDrag.generated.h"

class UCPP_Slot;

UCLASS()
class UE_RPG_API USlotDrag : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	UCPP_Slot* WidgetRef = nullptr;

	int32 GetIndex();

	bool bFromEquipmentSlot = false;
	EEquipmentType EquipmentType = EEquipmentType::None;
};
