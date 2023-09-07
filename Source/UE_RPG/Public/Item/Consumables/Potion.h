// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Item/ItemAbility.h"
#include "Potion.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API APotion : public AActor , public IItemAbility
{
	GENERATED_BODY()

public:

	virtual void UseItem(class ACPP_Character* player, const uint32 consumeValue) override;

	
};
