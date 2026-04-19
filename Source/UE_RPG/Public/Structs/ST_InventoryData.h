#pragma once

#include "CoreMinimal.h"
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