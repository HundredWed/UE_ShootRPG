// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Controller.h"
#include "GameFramework/HUD.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Component/CPP_UIManager.h"
#include "Component/CPP_QuestMananger.h"
#include "Component/CPP_DialogueManager.h"

ACPP_Controller::ACPP_Controller()
{
    UIManager = CreateDefaultSubobject<UCPP_UIManager>(TEXT("UIManager"));
    DialogueManager = CreateDefaultSubobject<UCPP_DialogueManager>(TEXT("DialogueManager"));
    QuestMananger = CreateDefaultSubobject<UCPP_QuestMananger>(TEXT("QuestMananger"));
}

void ACPP_Controller::BeginPlay()
{
    Super::BeginPlay();

    CrosshairHUD = GetHUD();
	CrosshairHUD->bShowHUD = false;

    ChangeInteractionState(EPlayerIputMappingState::Default);
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

    switch (CurrentInteractionState)
    {
    case EPlayerIputMappingState::Default:
        Subsystem->AddMappingContext(InputMappingContexts[CurrentInteractionState], 0);
        break;

    case EPlayerIputMappingState::NPCTalking:
        Subsystem->AddMappingContext(InputMappingContexts[CurrentInteractionState], 0);
        break;
    }
}

void ACPP_Controller::RevertToPlayerWidget()
{
    UIManager->SetMainWidget(EWidgetType::Player);
}

void ACPP_Controller::SetDialogeWidget()
{
    UIManager->SetMainWidget(EWidgetType::NPCDialogue);
}

bool ACPP_Controller::ActiveDialogueSubBox()
{
    //UIManager->
    return false;
}

void ACPP_Controller::ActivateAnswerBox(bool activate)
{
    //UIManager->
}

void ACPP_Controller::UpdateDialogueText(const FText& text)
{
    UIManager->UpdateDialogueText(text);
}

void ACPP_Controller::SetAnswerBox(TArray<FAnswerDialogue> answers)
{
    UIManager->SetAnswerBox(answers);
}

void ACPP_Controller::SetQuest(const FName& npcID)
{
    if (QuestMananger->GetQusetList(npcID).Num() > 0)
    {
        //UIManager->UpdateQuestList(QuestMananger->GetQusetList(npcID));
    }
}

void ACPP_Controller::SelectedQuest(const FName& npcID, const FName& questId)
{
   QuestMananger->SelectedQuest(npcID, questId);
}

void ACPP_Controller::AddProgressQuest()
{
    QuestMananger->AddProgressQuest();
}

void ACPP_Controller::QuestClear()
{
    QuestMananger->QuestClear();
}

void ACPP_Controller::SetHUDVisibility(bool bshowHUD)
{
	if (IsValid(CrosshairHUD))
	{
		CrosshairHUD->bShowHUD = bshowHUD;
	}
}


