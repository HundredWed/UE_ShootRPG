// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CPP_DialogueSystem.h"
#include "Kismet/KismetStringLibrary.h"

UCPP_DialogueSystem::UCPP_DialogueSystem()
{
	
}

void UCPP_DialogueSystem::AddProgressQuest(FQuest quest)
{
	if (quest.QuestID.IsNone())
	{
		return;
	}

	quest.QusetState = EQuestState::EQS_InProgress;

	InProgressQuests.Add(quest);
	//AcceptedQuests.Add(quest.QuestID, quest);
}

void UCPP_DialogueSystem::QuestClear(const FName& questID)
{
	int32 index = 0;
	for (FQuest quest : InProgressQuests)
	{
		if (quest.QuestID == questID)
		{
			ClearQuests.Add(questID);
			InProgressQuests.RemoveAt(index);
		}
		index++;
	}
}

FQuest UCPP_DialogueSystem::CheckQuestContent(const FName& objectID, const int32 amount)
{
	//해당 ID를 필요로 하는 퀘스트가 있는지 확인
	//있다면 진행도 확인

	int32 index = 0;
	FQuest correspondQuest;
	for (FQuest quest : InProgressQuests)
	{
		if (quest.NeedObjectID == objectID)
		{
			correspondQuest = quest;
		}
		index++;
	}


	if(index == InProgressQuests.Num()-1)
		return FQuest();

	//진행도 확인
	EQuestType type = correspondQuest.QusetType;

	switch (type)
	{
	case EQuestType::EQT_GetItem:
		CheckProgressGetItmeType(correspondQuest, amount, index);
		break;
	case EQuestType::EQT_GoToNPC:
		break;
	case EQuestType::EQT_GoToSpace:
		break;
	case EQuestType::EQT_ComBat:
		
		break;
	default:
		break;
	}

	return correspondQuest;
}

TArray<FQuest> UCPP_DialogueSystem::GetQusetList(const FName& npcID)
{
	TArray<FQuest> qusetlists;

	FNPCQuests* qusets = QuestDataTable->FindRow<FNPCQuests>(npcID,"");

	if (qusets == nullptr)
		return TArray<FQuest>();

	for (FQuest quest : qusets->Quests)
	{
		if (CheckClear(quest.QuestID))
			continue;

		//선행퀘스트 완료 유무
		if (!quest.PrevQuestID.IsNone() && CheckClear(quest.PrevQuestID))
		{
			qusetlists.Add(CheckProgress(quest.QuestID));
			continue;
		}

		FQuest progressQuest = CheckProgress(quest.QuestID);

		if (progressQuest.QuestID.IsNone())
		{
			qusetlists.Add(quest);
		}
		else
		{
			qusetlists.Add(progressQuest);
		}
	}


	return qusetlists;
}

FTalkDialogue UCPP_DialogueSystem::GetTalkStruct(const FName& npcID)
{
	return *TalkDataTable->FindRow<FTalkDialogue>(npcID, "");
}

FTalkDialogue UCPP_DialogueSystem::GetQuestDialogueStruct(const FName& npcID)
{
	return *QuestDialogueDataTable->FindRow<FTalkDialogue>(npcID, "");
}

TArray<FQuest> UCPP_DialogueSystem::GetProgrssQusetsOfPlayer()
{
	return InProgressQuests;
}

FQuest UCPP_DialogueSystem::GetQuestInfo(const FName& npcID, FName questID)
{
	TArray<FQuest> quests = GetQusetList(npcID);
	int32 questIndex = CastQuestIndex(questID);

	if (quests.IsValidIndex(questIndex))
	{
		FQuest progress = CheckProgress(quests[questIndex].QuestID);
		if (!progress.QuestID.IsNone())
		{
			return progress;
		}
		else
		{
			return quests[questIndex];
		}
	}

	return FQuest();
}

FQuest UCPP_DialogueSystem::CheckProgress(const FName& questID)
{
	for (FQuest progress : InProgressQuests)
	{
		if (progress.QuestID == questID)
		{
			return progress;
		}
	}

	return FQuest();
}

bool UCPP_DialogueSystem::CheckClear(const FName& questID)
{
	for (FName id : ClearQuests)
	{
		if(id == questID)
			return true;
	}
	return false;
}

int32 UCPP_DialogueSystem::CastQuestIndex(FName questID)
{
	FString rowNameToString = questID.ToString();

	int32 index = UKismetStringLibrary::FindSubstring(rowNameToString, "N");

	FString questIndex = UKismetStringLibrary::GetSubstring(rowNameToString, 0, index);

	return FCString::Atoi(*questIndex);
}

void UCPP_DialogueSystem::CheckProgressGetItmeType(FQuest& quest, const int32 amount, const int32 index)
{
	if (quest.NeedCount == amount)
	{
		InProgressQuests[index].QusetState = EQuestState::EQS_ConditionClear;
		quest = InProgressQuests[index];
	}
}

void UCPP_DialogueSystem::CheckProgressGoToNPCType(FQuest& quest, const FName& npcID, const int32 index)
{
	if (quest.NeedObjectID == npcID)
	{
		InProgressQuests[index].QusetState = EQuestState::EQS_ConditionClear;
		quest = InProgressQuests[index];
	}
}

void UCPP_DialogueSystem::CheckProgressGoToSpaceType()
{
}

void UCPP_DialogueSystem::CheckProgressComBatType()
{
}
