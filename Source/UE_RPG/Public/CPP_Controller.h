// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/ST_NPC.h"
#include "Structs/ST_DialogueAnswer.h"
#include "CPP_Controller.generated.h"

UENUM(BlueprintType)
enum class EPlayerIputMappingState : uint8
{
	Default,
	NPCTalking
};

class UInputMappingContext;
class AHUD;
class UCPP_DialogueManager;
class UCPP_UIManager;
class UCPP_QuestMananger;
class UMainPanelWidget;

UCLASS()
class UE_RPG_API ACPP_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	ACPP_Controller();

	virtual void BeginPlay() override;

	void ChangeInteractionState(EPlayerIputMappingState newState);

	void NPCInteract(const FName& npcID);

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

private:

	/*UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<UInputMappingContext*> InputMappingContexts;*/

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EPlayerIputMappingState, UInputMappingContext*> InputMappingContexts;

	UPROPERTY()
	TObjectPtr<UCPP_UIManager> UIManager;

	EPlayerIputMappingState CurrentInteractionState = EPlayerIputMappingState::Default;

	AHUD* CrosshairHUD;

	bool bInteractEvent = false;

};
