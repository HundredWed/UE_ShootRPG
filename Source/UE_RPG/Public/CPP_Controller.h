// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Structs/ST_NPC.h"
#include "Structs/ST_DialogueAnswer.h"
#include "CPP_Controller.generated.h"

class UInputMappingContext;
class AHUD;
class UCPP_UIManager;
class UMainPanelWidget;
class UCPP_InventoryWidget;
class UInputAction;
class UCPP_DialogueSystem;




UENUM(BlueprintType)
enum class EPlayerIputMappingState : uint8
{
	None,
	Default,
	NPCTalking
};

UCLASS()
class UE_RPG_API ACPP_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	ACPP_Controller();

	virtual void BeginPlay() override;
	
	void ChangeInteractionState(EPlayerIputMappingState newState);
		
	void SetNPCInteract(const FName& npcID);

	UFUNCTION()
	void SetHUDVisibility(bool bshowHUD);

	bool ToggleQuestWindow();

	void ShowCursor();
	void HideCursor(); 

	//임시. 리펙토링 필수
	UCPP_InventoryWidget* GetInventoryWidget(); 

protected:

	virtual void SetupInputComponent() override;

private:
	UFUNCTION()
	void InteractEvent();

	UFUNCTION()
	void SetToDefaultInteractionState();

	void UpdatePlayerWidget();

private:

	/*UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<UInputMappingContext*> InputMappingContexts;*/

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EPlayerIputMappingState, UInputMappingContext*> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCPP_UIManager> UIManager;

	UPROPERTY()
	UCPP_DialogueSystem* DialogueSystem;

	/**Input*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;


	EPlayerIputMappingState CurrentInteractionState = EPlayerIputMappingState::Default;

	AHUD* CrosshairHUD;

	bool bInteractEvent = false;

};
