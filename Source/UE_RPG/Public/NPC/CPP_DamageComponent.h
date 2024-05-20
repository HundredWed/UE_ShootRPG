// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CPP_DamageComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_DamageComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	void UpdateDamageUI(const int32 amount);

private:
	UPROPERTY()
		class UCPP_DamageUI* DamageUI;
	
};
