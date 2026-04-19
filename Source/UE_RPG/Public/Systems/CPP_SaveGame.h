// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Structs/ST_TotalPlayerData.h"
#include "CPP_SaveGame.generated.h"


UCLASS()
class UE_RPG_API UCPP_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UCPP_SaveGame();

	UPROPERTY()
	FString SlotName;


	UPROPERTY()
	FTotalSaveData TotalSaveData;

};
