#include "Systems/CPP_QuestSubsystem.h"
#include "Kismet/KismetStringLibrary.h"

UCPP_QuestSubsystem::UCPP_QuestSubsystem()
{
}

void UCPP_QuestSubsystem::InitQuestSubsystem(const FName& currentNpcID)
{
	CurrentNPCID = currentNpcID;
}

void UCPP_QuestSubsystem::AddProgressQuest()
{
	if (CurrentQuest.QuestID.IsNone())
	{
		return;
	}

	CurrentQuest.QuestState = EQuestState::EQS_InProgress;

	InProgressQuests.Add(CurrentQuest);
	//AcceptedQuests.Add(quest.QuestID, quest);
}

void UCPP_QuestSubsystem::QuestClear()
{
	int32 index = 0;
	for (FQuest quest : InProgressQuests)
	{
		if (quest.QuestID == CurrentQuest.QuestID)
		{
			ClearQuests.Add(CurrentQuest.QuestID);
			InProgressQuests.RemoveAt(index);
		}
		index++;
	}
}

void UCPP_QuestSubsystem::QuestStateChange(EQuestState questState)
{
	switch (questState)
	{
	case EQuestState::EQS_Normal:
		break;
	case EQuestState::EQS_InProgress:
		AddProgressQuest();
		break;
	case EQuestState::EQS_ConditionClear:
		CurrentQuest.QuestState = EQuestState::EQS_ConditionClear;
		break;
	case EQuestState::EQS_Clear:
		QuestClear();
		break;
	default:
		break;
	}
}

FQuest UCPP_QuestSubsystem::CheckQuestContent(const FName& objectID, const int32 amount)
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


	if (index == InProgressQuests.Num() - 1)
		return FQuest();

	//진행도 확인
	EQuestType type = correspondQuest.QuestType;

	switch (type)
	{
	case EQuestType::EQT_GetItem:
		CheckProgressGetItemType(correspondQuest, amount, index);
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

TArray<FQuest> UCPP_QuestSubsystem::GetQuestList()
{
	TArray<FQuest> questLists;

	FNPCQuests* quests = QuestDataTable->FindRow<FNPCQuests>(CurrentNPCID, "");

	if (quests == nullptr)
		return TArray<FQuest>();

	for (FQuest quest : quests->Quests)
	{
		if (CheckClear(quest.QuestID))
			continue;

		//선행퀘스트 완료 유무
		if (!quest.PrevQuestID.IsNone() && CheckClear(quest.PrevQuestID))
		{
			questLists.Add(CheckProgress(quest.QuestID));
			continue;
		}

		FQuest progressQuest = CheckProgress(quest.QuestID);

		if (progressQuest.QuestID.IsNone())
		{
			questLists.Add(quest);
		}
		else
		{
			questLists.Add(progressQuest);
		}
	}


	return questLists;
}

TArray<FQuest> UCPP_QuestSubsystem::GetProgressQuestsOfPlayer()
{
	return InProgressQuests;
}

FQuest UCPP_QuestSubsystem::GetQuestInfo(const FName& questID)
{
	TArray<FQuest> quests = GetQuestList();
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

void UCPP_QuestSubsystem::SelectedQuest(const FName& questId)
{
	CurrentQuest = GetQuestInfo(questId);
}

FQuest UCPP_QuestSubsystem::CheckProgress(const FName& questID)
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

bool UCPP_QuestSubsystem::CheckClear(const FName& questID)
{
	for (FName id : ClearQuests)
	{
		if (id == questID)
			return true;
	}
	return false;
}

int32 UCPP_QuestSubsystem::CastQuestIndex(FName questID)
{
	FString rowNameToString = questID.ToString();

	int32 index = UKismetStringLibrary::FindSubstring(rowNameToString, "N");

	FString questIndex = UKismetStringLibrary::GetSubstring(rowNameToString, 0, index);

	return FCString::Atoi(*questIndex);
}

void UCPP_QuestSubsystem::CheckProgressGetItemType(FQuest& quest, const int32 amount, const int32 index)
{
	if (quest.NeedCount == amount)
	{
		InProgressQuests[index].QuestState = EQuestState::EQS_ConditionClear;
		quest = InProgressQuests[index];
	}
}

void UCPP_QuestSubsystem::CheckProgressGoToNPCType(FQuest& quest, const int32 index)
{
	if (quest.NeedObjectID == CurrentNPCID)
	{
		InProgressQuests[index].QuestState = EQuestState::EQS_ConditionClear;
		quest = InProgressQuests[index];
	}
}

void UCPP_QuestSubsystem::CheckProgressGoToSpaceType()
{
}

void UCPP_QuestSubsystem::CheckProgressComBatType()
{
}