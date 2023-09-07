// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_Controller.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API ACPP_Controller : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	class AHUD* CrosshairHUD;

	UFUNCTION()
		void SetHUDVisibility(bool bshowHUD);

	FORCEINLINE void ShowCursor(){
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}

	FORCEINLINE void HideCursor() {
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
};
