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
class UCPP_SaveDataSubsystem;
class ICPP_SavableInterface;

DECLARE_MULTICAST_DELEGATE(FOnScreenBlackEventDelegate);
DECLARE_MULTICAST_DELEGATE(FOnScreenRevealEventDelegate);


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
	
	UFUNCTION(BlueprintCallable)
	void ChangeInteractionState(EPlayerIputMappingState newState);
		
	void SetNPCInteract(const FName& npcID);

	UFUNCTION()
	void SetHUDVisibility(bool bshowHUD);

	void ShowCursor();
	void HideCursor(); 

	void SetupPlayer();
	UCPP_InventoryWidget* GetInventoryWidget(); 

	void HandlePlayerDeath();

	FOnScreenBlackEventDelegate OnScreenBlackEvent;
	FOnScreenRevealEventDelegate OnScreenRevealEvent;

protected:

	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;


	void RespawnPlayer();

private:

	UFUNCTION()
	void InteractEvent();

	UFUNCTION()
	void SetToDefaultInteractionState();

	void OnSaveBroadcastReceived();
private:

	/*UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<UInputMappingContext*> InputMappingContexts;*/

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EPlayerIputMappingState, UInputMappingContext*> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCPP_UIManager> UIManager;

	UPROPERTY()
	UCPP_DialogueSystem* DialogueSystem;

	UPROPERTY()
	UCPP_SaveDataSubsystem* SaveSubsystem;

	UPROPERTY()
	TArray<TScriptInterface<ICPP_SavableInterface>> CachedSavableInterfaces;

	/**Input*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;


	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float RespawnDelay = 3.0f;

	FTimerHandle RespawnTimerHandle;


	EPlayerIputMappingState CurrentInteractionState = EPlayerIputMappingState::Default;
	EPlayerIputMappingState PrevInteractionState = EPlayerIputMappingState::Default;

	AHUD* CrosshairHUD;

	bool bInteractEvent = false;

};
