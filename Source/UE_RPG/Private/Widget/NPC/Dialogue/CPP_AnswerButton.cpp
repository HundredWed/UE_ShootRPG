// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_AnswerButton.h"


void UCPP_AnswerButton::InitAnswerButton(const FAnswerDialogue& answer, bool questAnswer)
{
	UpdateButtonText(answer.Answer);
	AnswerInfo.ReturnRowName = answer.RowName;
	AnswerInfo.AnswerType = answer.AnswerType;
	AnswerInfo.bQuestAnswer = questAnswer;
}

void UCPP_AnswerButton::DialogueButtonEvent()
{
	Super::DialogueButtonEvent();

	UpdateButtonText(FText::FromString(TEXT("")));
	OnAnswerEvent.Execute(AnswerInfo);
}