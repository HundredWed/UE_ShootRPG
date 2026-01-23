// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Controller.h"
#include "GameFramework/HUD.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Component/CPP_UIManager.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Systems/CPP_DialogueSystem.h"
#include "CPP_Character.h"

ACPP_Controller::ACPP_Controller()
{
    UIManager = CreateDefaultSubobject<UCPP_UIManager>(TEXT("UIManager"));
}

void ACPP_Controller::BeginPlay()
{
    Super::BeginPlay();

    CrosshairHUD = GetHUD();
	CrosshairHUD->bShowHUD = false;

    ChangeInteractionState(EPlayerIputMappingState::Default);

    UpdatePlayerWidget();
}

void ACPP_Controller::UpdatePlayerWidget()
{
    ACPP_Character* player = Cast<ACPP_Character>(GetPawn());

    if (UIManager && player)
    {
        UIManager->RegisterPlayerCharacterToWidget(player);
    }
}

void ACPP_Controller::ChangeInteractionState(EPlayerIputMappingState newState)
{
    CurrentInteractionState = newState;


    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!Subsystem)
    {
        return;
    }

    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(InputMappingContexts[CurrentInteractionState], 0);

    switch (CurrentInteractionState)
    {
    case EPlayerIputMappingState::Default:
        bInteractEvent = false;
        break;
    case EPlayerIputMappingState::NPCTalking:
        break;
    }
}

void ACPP_Controller::NPCInteract(const FName& npcID)
{
    UGameInstance* GI = GetGameInstance();
    if (!IsValid(GI))
    {
        return;
    }

    UCPP_DialogueSystem* dialogue = GI->GetSubsystem<UCPP_DialogueSystem>();
    
    if (!bInteractEvent)
    {
        //TODO
        //상호작용 시 캐릭터 상태
        //
        // 
        
        if (dialogue)
        {
           dialogue->InitDialogue(npcID);
        }
        bInteractEvent = true;
    }
    else
    {
        if (dialogue)
        {
            dialogue->PrintDialogue();
        }
    }   
}

void ACPP_Controller::SetHUDVisibility(bool bshowHUD)
{
	if (IsValid(CrosshairHUD))
	{
		CrosshairHUD->bShowHUD = bshowHUD;
	}
}

UCPP_InventoryWidget* ACPP_Controller::GetInventoryWidget()
{
    return UIManager->GetInventoryWidget();
}


