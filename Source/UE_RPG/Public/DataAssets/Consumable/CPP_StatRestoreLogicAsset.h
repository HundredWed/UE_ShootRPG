// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/CPP_ConsumptionItemDataAsset.h"
#include "CharacterStates.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_StatRestoreLogicAsset.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_StatRestoreLogicAsset : public UCPP_ConsumptionItemDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    ERestoreTypes RestoreType;

    UPROPERTY(EditDefaultsOnly)
    float RestoreAmount;

    virtual bool ExecuteLogic(AActor* Target) override;
	
};
