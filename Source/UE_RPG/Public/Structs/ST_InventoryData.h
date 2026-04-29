#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/EquipmentData.h"
#include "ST_InventoryData.generated.h"


USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY()
	FName ItemID = NAME_None;

	UPROPERTY()
	int32 ItemAmount = 0;
};

USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FName EquipmentID = NAME_None;
};

USTRUCT(BlueprintType)
struct FInventoryTotalData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FInventorySlot> SlotsArrayData;
	UPROPERTY()
	TMap<EEquipmentType, FEquipmentSlot> EquipmentSlotData;
	UPROPERTY()
	float Weight = 0.f;
	UPROPERTY()
	int32 Gold = 0;
};