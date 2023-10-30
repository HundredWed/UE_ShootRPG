// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/WeaponAbiliys/WeaponAbilityBase.h"
#include "UE_RPG/UtilityMecro.h"
#include "RedRifleAbility.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API URedRifleAbility : public UWeaponAbilityBase
{
	GENERATED_BODY()
	
public:

	virtual void WeaponAbility() override;
};
