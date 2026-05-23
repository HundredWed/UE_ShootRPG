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

struct FDamageReceipt;

class UE_RPG_API IHitEventInterface
{
	GENERATED_BODY()

public:

	virtual void ExecuteHitEvent(FDamageReceipt& receipt, AController* eventInstigator, AActor* damageCauser) {}
};
