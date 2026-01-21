// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_DialogueCategoryButton.h"
#include "Systems/CPP_DialogueSystem.h"

void UCPP_DialogueCategoryButton::DialogueButtonEvent()
{
	Super::DialogueButtonEvent();

	UGameInstance* GI = GetGameInstance();
	if (!IsValid(GI))
	{
		return;
	}

	UCPP_DialogueSystem* dialogue = GI->GetSubsystem<UCPP_DialogueSystem>();
	if (!IsValid(dialogue))
	{
		return;
	}

	dialogue->SelectedInteractType(InteractType);
}
