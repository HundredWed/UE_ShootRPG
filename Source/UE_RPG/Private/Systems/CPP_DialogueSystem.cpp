// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CPP_DialogueSystem.h"
#include "Kismet/KismetStringLibrary.h"
#include "Systems/CPP_MyGameSettings.h"

UCPP_DialogueSystem::UCPP_DialogueSystem()
{
	
}

void UCPP_DialogueSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UCPP_MyGameSettings* Settings = GetDefault<UCPP_MyGameSettings>();

	TalkDataTable = UCPP_MyGameSettings::LoadDataTableSafely(Settings->TalkData);
	QuestDialogueDataTable = UCPP_MyGameSettings::LoadDataTableSafely(Settings->QuestDialogueData);
	NPCDataTable = UCPP_MyGameSettings::LoadDataTableSafely(Settings->NPCData);
}

void UCPP_DialogueSystem::InitDialogue(const FName& dialogueOwnerName)
{
	bActivateAnswerBox = false;
	bSelectedQuest = false;
	bTalkEnd = false;
	DialogueState = EInteractType::Normal;

	DialogueOwnerName = dialogueOwnerName;
	DatatableRowName = dialogueOwnerName;
}

void UCPP_DialogueSystem::PrintDialogue()
{
	if (bActivateAnswerBox)
	{
		return;
	}

	switch (DialogueState)
	{
	case EInteractType::Normal:
		PrintDialogueNormal();
		break;
	case EInteractType::JustTalk:
		PrintDialogueJustTalk();
		break;
	case EInteractType::Quest:
		PrintDialogueQuest();
		break;
	case EInteractType::LikeAbility:
		PrintDialogueLikeAbility();
		break;
	case EInteractType::Revert:
		break;
	case EInteractType::Quit:
		PrintDialogueQuit();
		break;
	default:
		break;
	}
}

void UCPP_DialogueSystem::SelectedInteractType(EInteractType state)
{
	switch (state)
	{
	case EInteractType::Normal:
		break;
	case EInteractType::JustTalk:
		DialogueState = EInteractType::JustTalk;
		PrintDialogue();
		break;
	case EInteractType::Quest:
		DialogueState = EInteractType::Quest;
		break;
	case EInteractType::LikeAbility:
		break;
	case EInteractType::Revert:
		RevertDialogue();
		PrintDialogue();
		break;
	case EInteractType::Quit:
		OnQuitDialogue.Execute();
		break;
	default:
		break;
	}

}

void UCPP_DialogueSystem::SelectedAnswer(const FName& rowName)
{
	DatatableRowName = rowName;
	bActivateAnswerBox = false;
	PrintDialogue();
}

void UCPP_DialogueSystem::SelectedQuest(const FQuest& quest)
{
	bSelectedQuest = true;

	EQuestState questState = quest.QuestState;
	DatatableRowName = quest.QuestID;

	switch (questState)
	{
	case EQuestState::EQS_Normal:
		DatatableRowName = quest.QuestID;
		break;
	case EQuestState::EQS_InProgress:
		DatatableRowName = quest.QuestProgressRow;
		break;
	case EQuestState::EQS_ConditionClear:
		DatatableRowName = quest.QuestClearRow;
		break;
	case EQuestState::EQS_Clear:
		break;
	default:
		break;
	}

	PrintDialogue();
}

void UCPP_DialogueSystem::RevertDialogue()
{
	InitDialogue(DialogueOwnerName);
	EndDialogue.Execute();
}

void UCPP_DialogueSystem::PrintDialogueNormal()
{
}

void UCPP_DialogueSystem::PrintDialogueJustTalk()
{
	if (bTalkEnd)
	{
		RevertDialogue();
	}
	else
	{
		FTalkDialogue talk = GetTalkStruct();

		if (talk.Dialogue.IsEmpty())
		{
			return;
		}

		UpdateDialogueText.Execute(talk.Dialogue, talk.DialogueEventType);

		if (talk.Answers.Num() > 0)
		{
			OnUpdateAnswerBox.Execute(talk.Answers, false);
			bActivateAnswerBox = true;
		}
		else
		{
			DatatableRowName = talk.NextRow;
			bTalkEnd = (talk.NextRow.IsNone()) && (talk.Answers.IsEmpty());
		}
	}
}

void UCPP_DialogueSystem::PrintDialogueQuest()
{
	if (bTalkEnd && bSelectedQuest)
	{
		RevertDialogue();
	}
	else
	{
		FTalkDialogue quest = GetQuestDialogueStruct();

		if (quest.Dialogue.IsEmpty())
		{
			return;
		}

		UpdateDialogueText.Execute(quest.Dialogue, quest.DialogueEventType);

		if (quest.Answers.Num() > 0)
		{
			OnUpdateAnswerBox.Execute(quest.Answers, true);
			bActivateAnswerBox = true;
		}
		else
		{
			DatatableRowName = quest.NextRow;
			bTalkEnd = (quest.NextRow.IsNone()) && (quest.Answers.IsEmpty());
		}
	}
}

void UCPP_DialogueSystem::PrintDialogueLikeAbility()
{
}

void UCPP_DialogueSystem::PrintDialogueQuit()
{
}

bool UCPP_DialogueSystem::IsEndDialogue()
{
	return false;
}

FTalkDialogue UCPP_DialogueSystem::GetTalkStruct()
{
	if (FTalkDialogue* data = TalkDataTable->FindRow<FTalkDialogue>(DatatableRowName, ""))
	{
		return *data;
	}
	else
	{
		WARNINGLOG(TEXT("TalkDataTable의 ID가 유효하지 않습니다!"));
		return FTalkDialogue();
	}
}

FTalkDialogue UCPP_DialogueSystem::GetQuestDialogueStruct()
{
	if (FTalkDialogue* data = QuestDialogueDataTable->FindRow<FTalkDialogue>(DatatableRowName, ""))
	{
		return *data;
	}
	else
	{
		WARNINGLOG(TEXT("QuestDialogueDataTable의 ID가 유효하지 않습니다!"));
		return FTalkDialogue();
	}
}

FNPCDialogue UCPP_DialogueSystem::GetNPCStruct()
{
	if (FNPCDialogue* data = NPCDataTable->FindRow<FNPCDialogue>(DatatableRowName, ""))
	{
		return *data;
	}
	else
	{
		WARNINGLOG(TEXT("NPCDataTable의 ID가 유효하지 않습니다!"));
		return FNPCDialogue();
	}
}