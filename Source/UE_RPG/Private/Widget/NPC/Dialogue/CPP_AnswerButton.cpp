// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_AnswerButton.h"
#include "Systems/CPP_DialogueSystem.h"
#include "Systems/CPP_QuestSubsystem.h"

void UCPP_AnswerButton::InitAnswerButton(const FAnswerDialogue& answer, bool questAnswer)
{
	UpdateButtonText(answer.Answer);
	ReturnRowName = answer.RowName;
	AnswerType = answer.AnswerType;
	bQuestAnswer = questAnswer;
}

void UCPP_AnswerButton::DialogueButtonEvent()
{
	Super::DialogueButtonEvent();

	UGameInstance* GI = GetGameInstance();
	if (!IsValid(GI))
	{
		return;
	}

	UCPP_DialogueSystem* dialogue = GI->GetSubsystem<UCPP_DialogueSystem>();
	if (IsValid(dialogue))
	{
		dialogue->SelectedAnswer(ReturnRowName);
	}
	

	UCPP_QuestSubsystem* quest = GI->GetSubsystem<UCPP_QuestSubsystem>();
	if (bQuestAnswer && IsValid(quest))
	{
		quest->AddProgressQuest();
	}


	OnAnswerEvent.Execute();
}
