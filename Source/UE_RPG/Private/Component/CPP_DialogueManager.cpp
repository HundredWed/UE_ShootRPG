#include "Component/CPP_DialogueManager.h"
#include "GameInstance/CPP_CustomInstance.h"
#include "CPP_Controller.h"

UCPP_DialogueManager::UCPP_DialogueManager()
{

}

void UCPP_DialogueManager::InitDialogue(const FName& dialogueOwnerName)
{
	PlayerController->SetDialogeWidget();

	bActivateAnswerBox = false;
	bSelectedQuest = false;
	bTalkEnd = false;
	DialogueState = EDialogueState::Normal;

	DialogueOwnerName = dialogueOwnerName;
	DatatableRowName = dialogueOwnerName;
}

void UCPP_DialogueManager::PrintDialogue()
{
	if (bActivateAnswerBox)
	{
		return;
	}

	switch (DialogueState)
	{
	case EDialogueState::Normal:
		PrintDialogueNormal();
		break;
	case EDialogueState::JustTalk:
		PrintDialogueJustTalk();
		break;
	case EDialogueState::Quest:
		PrintDialogueQuest();
		break;
	case EDialogueState::LikeAbility:
		PrintDialogueLikeAbility();
		break;
	case EDialogueState::Quit:
		PrintDialogueQuit();
		break;
	default:
		break;
	}
}

void UCPP_DialogueManager::SelectedInteractType(EDialogueState state)
{
	switch (state)
	{
	case EDialogueState::Normal:
		break;
	case EDialogueState::JustTalk:
		DialogueState = EDialogueState::JustTalk;
		break;
	case EDialogueState::Quest:
		SetQuestList();
		break;
	case EDialogueState::LikeAbility:
		break;
	case EDialogueState::Quit:
		RevertToMain();
		break;
	default:
		break;
	}
}

void UCPP_DialogueManager::SelectedAnswer(const FName& rowName, bool questAccept)
{
	if (questAccept)
	{
		PlayerController->AddProgressQuest();
	}

	DatatableRowName = rowName;
	bActivateAnswerBox = false;
	PlayerController->ActivateAnswerBox(bActivateAnswerBox);
	PrintDialogue();
}

void UCPP_DialogueManager::SelectedQuest(const FQuest& quest)
{
	bSelectedQuest = true;

	EQuestState qusetState = quest.QusetState;
	DatatableRowName = quest.QuestID;

	switch (qusetState)
	{
	case EQuestState::EQS_Normal:
		DatatableRowName = quest.QuestID;
		break;
	case EQuestState::EQS_InProgress:
		DatatableRowName = GetQuestDialogueStruct().QuestProgressRow;
		break;
	case EQuestState::EQS_ConditionClear:
		PlayerController->QuestClear();
		DatatableRowName = GetQuestDialogueStruct().QuestClearRow;
		break;
	case EQuestState::EQS_Clear:
		break;
	default:
		break;
	}

	PrintDialogue();
	PlayerController->ActivateAnswerBox(bActivateAnswerBox);
}

void UCPP_DialogueManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ACPP_Controller>(GetOwner());
}

void UCPP_DialogueManager::RevertDialogue()
{
	InitDialogue(DialogueOwnerName);
}

void UCPP_DialogueManager::RevertToMain()
{
	PlayerController->RevertToPlayerWidget();
}

void UCPP_DialogueManager::SetQuestList()
{
	DialogueState = EDialogueState::Quest;
	PlayerController->ActivateAnswerBox(bActivateAnswerBox);
	PlayerController->SetQuest(DialogueOwnerName);
}

void UCPP_DialogueManager::SetAnswerBox(const FTalkDialogue& dialouge)
{
	PlayerController->SetAnswerBox(dialouge.Answers);

	bActivateAnswerBox = true;
	PlayerController->ActivateAnswerBox(bActivateAnswerBox);
}

void UCPP_DialogueManager::PrintDialogueNormal()
{
}

void UCPP_DialogueManager::PrintDialogueJustTalk()
{
	if (bTalkEnd)
	{
		RevertDialogue();
	}
	else
	{
		PlayerController->ActiveDialogueSubBox();
		FTalkDialogue talk = GetTalkStruct();

		PlayerController->UpdateDialogueText(talk.Dialogue);

		if (talk.Answers.Num() > 0)
		{
			SetAnswerBox(talk);
		}
		else
		{
			DatatableRowName = talk.NextRow;
			bTalkEnd = talk.EndDialogue;
		}		
	}
}

void UCPP_DialogueManager::PrintDialogueQuest()
{
	if (bTalkEnd && !bSelectedQuest)
	{
		RevertDialogue();
	}
	else
	{
		FTalkDialogue quest = GetQuestDialogueStruct();
		PlayerController->UpdateDialogueText(quest.Dialogue);

		if (quest.Answers.Num() > 0)
		{
			SetAnswerBox(quest);
		}
		else
		{
			DatatableRowName = quest.NextRow;
			bTalkEnd = quest.EndDialogue;
		}
	}
}

void UCPP_DialogueManager::PrintDialogueLikeAbility()
{
}

void UCPP_DialogueManager::PrintDialogueQuit()
{
}

FTalkDialogue UCPP_DialogueManager::GetTalkStruct()
{
	return *TalkDataTable->FindRow<FTalkDialogue>(DatatableRowName, "");
}

FTalkDialogue UCPP_DialogueManager::GetQuestDialogueStruct()
{
	return *QuestDialogueDataTable->FindRow<FTalkDialogue>(DatatableRowName, "");
}

FNPCDialogue UCPP_DialogueManager::GetNPCStruct()
{
	return *NPCDataTable->FindRow<FNPCDialogue>(DialogueOwnerName, "");
}


