// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SystemSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API USystemSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, Category = "SystemData")
	FString LatestSaveSlotName = TEXT("Slot_A");
};
