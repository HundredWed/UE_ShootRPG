// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_DialogueCategoryButton.h"
#include "Systems/CPP_DialogueSystem.h"

void UCPP_DialogueCategoryButton::InitType(EInteractType interactType)
{
	InteractType = interactType;

	switch (InteractType)
	{
	case EInteractType::JustTalk:
		UpdateButtonText(FText::FromString(TEXT("대화")));
		break;
	case EInteractType::Quest:
		UpdateButtonText(FText::FromString(TEXT("퀘스트")));
		break;
	case EInteractType::LikeAbility:
		UpdateButtonText(FText::FromString(TEXT("호감도")));
		break;
	case EInteractType::Revert:
		UpdateButtonText(FText::FromString(TEXT("처음으로")));
		break;
	case EInteractType::Quit:
		UpdateButtonText(FText::FromString(TEXT("나가기")));
		break;
	default:
		break;
	}
}

void UCPP_DialogueCategoryButton::DialogueButtonEvent()
{
	Super::DialogueButtonEvent();

	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_DialogueSystem* dialogue = GI->GetSubsystem<UCPP_DialogueSystem>();
		dialogue->SelectedInteractType(InteractType);
	}	

	OnInteractButtonEvent.Execute(InteractType);
}
