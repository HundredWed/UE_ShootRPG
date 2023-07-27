// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "GrabbableItem.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API AGrabbableItem : public AItem
{
	GENERATED_BODY()
	
public:
	AGrabbableItem();

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* RootMesh;

};
