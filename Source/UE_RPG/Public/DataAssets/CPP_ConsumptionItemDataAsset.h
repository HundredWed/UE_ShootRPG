// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CPP_ConsumptionItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Abstract)
class UE_RPG_API UCPP_ConsumptionItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:


	virtual void ExecuteLogic(AActor* target) PURE_VIRTUAL(UCPP_ConsumptionItemDataAsset::ExecuteLogic, );
};
