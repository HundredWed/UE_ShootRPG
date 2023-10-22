// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UNPCHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	bool IsValideHealthBar();

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
};
