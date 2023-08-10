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

	virtual void BeginPlay() override;

	UFUNCTION()
		void SetStateEquiped() { SetItemState(EItemState::EIS_Equipped); }
	UFUNCTION()
		void SetStateUnEquiped() { SetItemState(EItemState::EIS_UnEquipped); }

protected:
	virtual void SetItemState(EItemState State) override;
};
