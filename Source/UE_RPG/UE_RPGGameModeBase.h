// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE_RPG/UtilityMecro.h"
#include "UE_RPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API AUE_RPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void StartPlay();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	FTimerHandle AutoSaveTimerHandle;

	void TriggerAutoSave();

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float AutoSaveDelay = 180.f;
};
