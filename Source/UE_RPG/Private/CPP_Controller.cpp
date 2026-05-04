// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Controller.h"
#include "GameFramework/HUD.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

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

    DialogueSystem = GetGameInstance()->GetSubsystem<UCPP_DialogueSystem>();
    check(DialogueSystem);   
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

void ACPP_Controller::SetupPlayer()
{
    ACPP_Character* player = Cast<ACPP_Character>(GetPawn());
    if (player)
    {
        UIManager->InitUIManager();
        UIManager->RegisterPlayerCharacterToWidget(player);
        player->InitInventory(this);
        player->InitCharacterStat(true);
    }   
}

void ACPP_Controller::OnSaveBroadcastReceived()
{
    for (auto& savable : CachedSavableInterfaces)
    {
        if (savable)
        {
            savable->GatherSaveData(SaveSubsystem);
        }
    }
}

void ACPP_Controller::ChangeInteractionState(EPlayerIputMappingState newState)
{
    PrevInteractionState = CurrentInteractionState;
    CurrentInteractionState = newState;


    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!Subsystem)
    {
        return;
    }

    Subsystem->ClearAllMappings();

    if (CurrentInteractionState != EPlayerIputMappingState::None)
    {
        if (InputMappingContexts.Contains(CurrentInteractionState))
        {
            Subsystem->AddMappingContext(InputMappingContexts[CurrentInteractionState], 0);
        }
    }

    switch (CurrentInteractionState)
    {
    case EPlayerIputMappingState::None:
        OnScreenBlackEvent.Broadcast();
        HideCursor();
        break;
    case EPlayerIputMappingState::Default:
        OnScreenRevealEvent.Broadcast();
        bInteractEvent = false;
        HideCursor();
        if (PrevInteractionState == EPlayerIputMappingState::NPCTalking)
        {
            Cast<ACPP_Character>(GetPawn())->EndDialogueCamera();
            Cast<ACPP_Character>(GetPawn())->SetHiddenPlayer(false);
        }
        break;
    case EPlayerIputMappingState::NPCTalking:
        ShowCursor();
        Cast<ACPP_Character>(GetPawn())->SetHiddenPlayer(true);
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

void ACPP_Controller::OnPossess(APawn* aPawn)
{
    UGameInstance* GI = GetGameInstance();
    if (!IsValid(GI))
    {
        return;
    }

    TArray<UActorComponent*> comps = aPawn->GetComponentsByInterface(UCPP_SavableInterface::StaticClass());

    CachedSavableInterfaces.Empty();
    for (UActorComponent* comp : comps)
    {
        if (comp->Implements<UCPP_SavableInterface>())
        {
            CachedSavableInterfaces.Add(comp);
        }
    }

    if(SaveSubsystem == nullptr)
        SaveSubsystem = GI->GetSubsystem<UCPP_SaveDataSubsystem>();

    check(SaveSubsystem);

    SaveSubsystem->OnGatherSaveData.RemoveAll(this);
    SaveSubsystem->OnGatherSaveData.AddUObject(this, &ACPP_Controller::OnSaveBroadcastReceived);

    for (auto& SavableComp : CachedSavableInterfaces)
    {
        if (SavableComp)
        {
            SavableComp->ApplySaveData(SaveSubsystem);
        }        
    }

    Super::OnPossess(aPawn);

    //GetPawn() 유효구간
    SetupPlayer();
}

void ACPP_Controller::HandlePlayerDeath()
{
    ChangeInteractionState(EPlayerIputMappingState::None);
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ACPP_Controller::RespawnPlayer, RespawnDelay, false);
}

void ACPP_Controller::RespawnPlayer()
{
    SaveSubsystem->SaveGameData();

    APawn* DeadPawn = GetPawn();
    if (DeadPawn)
    {
        UnPossess();
        DeadPawn->Destroy();
    }

    AActor* spawnPoint = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
    FVector spawnLocation = spawnPoint ? spawnPoint->GetActorLocation() : FVector(0, 0, 100);
    FRotator spawnRotation = spawnPoint ? spawnPoint->GetActorRotation() : FRotator::ZeroRotator;

    AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
    UClass* characterClass = GameMode ? GameMode->GetDefaultPawnClassForController(this) : nullptr;
    if (characterClass)
    {
        APawn* NewPawn = GetWorld()->SpawnActor<APawn>(characterClass, spawnLocation, spawnRotation);

        if (NewPawn)
        {
            Possess(NewPawn);
            ChangeInteractionState(EPlayerIputMappingState::Default);
        }
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

void ACPP_Controller::ShowCursor()
{
    if (bShowMouseCursor) return;

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
    if (!bShowMouseCursor) return;

    SetInputMode(FInputModeGameOnly());
    bShowMouseCursor = false;
}
