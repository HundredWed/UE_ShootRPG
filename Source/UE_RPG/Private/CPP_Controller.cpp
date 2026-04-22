// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Controller.h"
#include "GameFramework/HUD.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Component/CPP_UIManager.h"
#include "Systems/CPP_DialogueSystem.h"
#include "CPP_Character.h"
#include "Interface/CPP_SavableInterface.h"
#include "Systems/CPP_SaveDataSubsystem.h"

ACPP_Controller::ACPP_Controller()
{
    UIManager = CreateDefaultSubobject<UCPP_UIManager>(TEXT("UIManager"));
    UIManager->RevertPlayerWidget.BindUObject(this, &ACPP_Controller::SetToDefaultInteractionState);
}

void ACPP_Controller::BeginPlay()
{
    Super::BeginPlay();

    CrosshairHUD = GetHUD();
	CrosshairHUD->bShowHUD = false;

    SetToDefaultInteractionState();

    UpdatePlayerWidget();

    UGameInstance* GI = GetGameInstance();
    if (!IsValid(GI))
    {
        return;
    }

    TArray<UActorComponent*> comps = GetComponentsByInterface(UCPP_SavableInterface::StaticClass());
    for (UActorComponent* comp : comps)
    {
        ICPP_SavableInterface* savable = Cast<ICPP_SavableInterface>(comp);
        if (savable)
        {
            CachedSavableInterfaces.Add(savable);
        }
    }

    DialogueSystem = GI->GetSubsystem<UCPP_DialogueSystem>();
    check(DialogueSystem);

    SaveSubsystem = GetGameInstance()->GetSubsystem<UCPP_SaveDataSubsystem>();
    check(SaveSubsystem);
    SaveSubsystem->OnGatherSaveData.AddUObject(this, &ACPP_Controller::OnSaveBroadcastReceived);

    if (SaveSubsystem->IsDataReady())
    {
        for (ICPP_SavableInterface* SavableComp : CachedSavableInterfaces)
        {
            SavableComp->ApplySaveData(SaveSubsystem);
        }
    }
}

void ACPP_Controller::InteractEvent()
{
    switch (CurrentInteractionState)
    {
    case EPlayerIputMappingState::Default:
        break;
    case EPlayerIputMappingState::NPCTalking:
        DialogueSystem->PrintDialogue();
        break;
    }   
}

void ACPP_Controller::SetToDefaultInteractionState()
{
    ChangeInteractionState(EPlayerIputMappingState::Default);
}

void ACPP_Controller::UpdatePlayerWidget()
{
    ACPP_Character* player = Cast<ACPP_Character>(GetPawn());

    if (UIManager && player)
    {
        UIManager->RegisterPlayerCharacterToWidget(player->StatComponent);
    }
}

void ACPP_Controller::OnSaveBroadcastReceived()
{
    for (ICPP_SavableInterface* savable : CachedSavableInterfaces)
    {
        savable->GatherSaveData(SaveSubsystem);
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
        HideCursor();
        break;
    case EPlayerIputMappingState::NPCTalking:
        ShowCursor();
        break;
    }
}

void ACPP_Controller::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACPP_Controller::InteractEvent);
    }
}

void ACPP_Controller::SetNPCInteract(const FName& npcID)
{
    DialogueSystem->InitDialogue(npcID);
    UIManager->SetMainWidget(EWidgetType::NPCDialogue);
    bInteractEvent = true;
    ChangeInteractionState(EPlayerIputMappingState::NPCTalking);
}

void ACPP_Controller::SetHUDVisibility(bool bshowHUD)
{
	if (IsValid(CrosshairHUD))
	{
		CrosshairHUD->bShowHUD = bshowHUD;
	}
}

bool ACPP_Controller::ToggleQuestWindow()
{
    bool isActivate = UIManager->ToggleQuestListWindow();

    if (isActivate)
    {
        ShowCursor();
    }
    else
    {
        HideCursor();
    }

    return isActivate;
}

void ACPP_Controller::ShowCursor()
{
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);
    bShowMouseCursor = true;
}

UCPP_InventoryWidget* ACPP_Controller::GetInventoryWidget()
{
    return UIManager->GetInventoryWidget();
}

void ACPP_Controller::HideCursor()
{
    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
}
