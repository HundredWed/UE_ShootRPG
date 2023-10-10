// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UWeaponAbilityBase : public UObject
{
	GENERATED_BODY()
	
public:

	virtual void WeaponAbility();

	static UWeaponAbilityBase* GetAbility(const uint8 id);
};
