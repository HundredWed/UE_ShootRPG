// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CPP_DialogueSystem.h"
#include "Kismet/KismetStringLibrary.h"

UCPP_DialogueSystem::UCPP_DialogueSystem()
{
	
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
		break;
	case EInteractType::Quest:
		DialogueState = EInteractType::Quest;
		break;
	case EInteractType::LikeAbility:
		break;
	case EInteractType::Revert:
		RevertDialogue();
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
		DatatableRowName = GetQuestDialogueStruct().QuestProgressRow;
		break;
	case EQuestState::EQS_ConditionClear:
		DatatableRowName = GetQuestDialogueStruct().QuestClearRow;
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
		//PlayerController->ActivateDialogueSubBox(true);->ui ³»¿¡¼­
		FTalkDialogue talk = GetTalkStruct();
		UpdateDialogueText.Execute(talk.Dialogue);

		if (talk.Answers.Num() > 0)
		{
			CreateAnswerBox.Execute(talk.Answers);
			bActivateAnswerBox = true;
		}
		else
		{
			DatatableRowName = talk.NextRow;
			bTalkEnd = talk.EndDialogue;
		}
	}
}

void UCPP_DialogueSystem::PrintDialogueQuest()
{
	if (bTalkEnd && !bSelectedQuest)
	{
		RevertDialogue();
	}
	else
	{
		FTalkDialogue quest = GetQuestDialogueStruct();
		UpdateDialogueText.Execute(quest.Dialogue);

		if (quest.Answers.Num() > 0)
		{
			CreateAnswerBox.Execute(quest.Answers);
			bActivateAnswerBox = true;
		}
		else
		{
			DatatableRowName = quest.NextRow;
			bTalkEnd = quest.EndDialogue;
		}
	}
}

void UCPP_DialogueSystem::PrintDialogueLikeAbility()
{
}

void UCPP_DialogueSystem::PrintDialogueQuit()
{
}

FTalkDialogue UCPP_DialogueSystem::GetTalkStruct()
{
	return *TalkDataTable->FindRow<FTalkDialogue>(DatatableRowName, "");
}

FTalkDialogue UCPP_DialogueSystem::GetQuestDialogueStruct()
{
	return *QuestDialogueDataTable->FindRow<FTalkDialogue>(DatatableRowName, "");
}

FNPCDialogue UCPP_DialogueSystem::GetNPCStruct()
{
	return *NPCDataTable->FindRow<FNPCDialogue>(DialogueOwnerName, "");
}