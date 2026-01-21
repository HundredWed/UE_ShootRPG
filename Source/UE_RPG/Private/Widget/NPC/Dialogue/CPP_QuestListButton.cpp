// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_QuestListButton.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Systems/CPP_DialogueSystem.h"

void UCPP_QuestListButton::InitQuestListButton(const FQuest& questInfo)
{
	QuestInfo = questInfo;

	FText questName = QuestInfo.QuestName;
	FText FormatPattern = FText::FromString(TEXT("{0}({1})"));

	switch (QuestInfo.QuestState)
	{
	case EQuestState::EQS_InProgress:
		questName = FText::Format(FormatPattern, questName, FText::FromString(TEXT("진행중")));
		break;
	case EQuestState::EQS_ConditionClear:
		questName = FText::Format(FormatPattern, questName, FText::FromString(TEXT("완료가능")));
		break;
	}

	UpdateButtonText(questName);
}

void UCPP_QuestListButton::DialogueButtonEvent()
{
	Super::DialogueButtonEvent();

	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* QS = GI->GetSubsystem<UCPP_QuestSubsystem>();
		QS->SelectedQuest(QuestInfo.QuestID);

		UCPP_DialogueSystem* DS = GI->GetSubsystem<UCPP_DialogueSystem>();
		DS->SelectedQuest(QuestInfo);
	}
}
