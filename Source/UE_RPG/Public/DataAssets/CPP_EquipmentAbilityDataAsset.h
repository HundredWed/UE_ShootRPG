// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CPP_EquipmentAbilityDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Abstract)
class UE_RPG_API UCPP_EquipmentAbilityDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	virtual void ExecuteAbility(AActor* Target) const PURE_VIRTUAL(UCPP_EquipmentAbilityDataAsset::ExecuteAbility, );
};
