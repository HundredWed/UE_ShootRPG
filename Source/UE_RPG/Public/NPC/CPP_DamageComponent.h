// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Structs/ST_DamageFeedback.h"
#include "CPP_DamageComponent.generated.h"

class UCPP_DamageUI;

UCLASS()
class UE_RPG_API UCPP_DamageComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	void UpdateDamageUI(const float amount, EDamageType type);
	
	bool IsActivateUI();

private:
	UPROPERTY()
	UCPP_DamageUI* DamageUI;
	
};
