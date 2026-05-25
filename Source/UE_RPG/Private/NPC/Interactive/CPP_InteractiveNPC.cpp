// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Interactive/CPP_InteractiveNPC.h"
#include "Camera/CameraComponent.h"
#include "CPP_Character.h"

#include "Systems/CPP_QuestSubsystem.h"

ACPP_InteractiveNPC::ACPP_InteractiveNPC()
{
	DialogueCameraPreview = CreateDefaultSubobject<UCameraComponent>(TEXT("DialogueCameraPreview"));
	DialogueCameraPreview->SetupAttachment(GetRootComponent());
	DialogueCameraPreview->bIsEditorOnly = true;
}

void ACPP_InteractiveNPC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR

	if (DialogueCameraPreview)
	{
		CachedCameraTransform = DialogueCameraPreview->GetComponentTransform();
	}

#endif
}

void ACPP_InteractiveNPC::RequestInteract(AActor* interactor)
{
	if (ACPP_Character* character = Cast<ACPP_Character>(interactor))
	{
		InitQuestSystem();
		character->SetDialogue(NPCID, CachedCameraTransform);
	}
}

void ACPP_InteractiveNPC::BeginPlay()
{
	Super::BeginPlay();

	CharacterType = ECharacterTypes::NPC_Talkable;
}

void ACPP_InteractiveNPC::InitQuestSystem()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* quest = GI->GetSubsystem<UCPP_QuestSubsystem>();
		quest->InitQuestSubsystem(NPCID);
	}
}
