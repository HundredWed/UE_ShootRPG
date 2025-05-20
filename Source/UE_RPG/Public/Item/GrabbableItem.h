// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/PickUpItem.h"
#include "GrabbableItem.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API AGrabbableItem : public APickUpItem
{
	GENERATED_BODY()
	
public:
	AGrabbableItem();

	virtual void BeginPlay() override;

};
