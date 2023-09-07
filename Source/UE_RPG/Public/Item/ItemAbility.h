// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemAbility.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_RPG_API IItemAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void UseItem(class ACPP_Character* player, const uint32 consumeValue) = 0;
};
