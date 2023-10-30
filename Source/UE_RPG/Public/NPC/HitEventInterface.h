// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitEventInterface.generated.h"


UINTERFACE(MinimalAPI)
class UHitEventInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_RPG_API IHitEventInterface
{
	GENERATED_BODY()

public:

	virtual bool GetHit(const FVector& hitDir) = 0;
};
