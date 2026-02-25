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

	InProgressQuests.Add(CurrentQuest.QuestID, CurrentQuest);
}

void UCPP_QuestSubsystem::QuestClear()
{
	ClearQuests.Add(CurrentQuest.QuestID);
	InProgressQuests.Remove(CurrentQuest.QuestID);
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

FQuest UCPP_QuestSubsystem::CheckQuestContent(const FName& objectID, int32 amount)
{
	//해당 ID를 필요로 하는 퀘스트가 있는지 확인
	//있다면 진행도 확인

	FQuest correspondQuest;
	for (auto quest : InProgressQuests)
	{
		if (quest.Value.NeedObjectID == objectID)
		{
			correspondQuest = quest.Value;
		}
	}


	if (correspondQuest.QuestID.IsNone())
	{
		return FQuest();
	}
		
	//진행도 확인
	EQuestType type = correspondQuest.QuestType;

	switch (type)
	{
	case EQuestType::EQT_GetItem:
		CheckProgressAmountType(correspondQuest, amount);
		break;
	case EQuestType::EQT_ComBat:
		CheckProgressAmountType(correspondQuest, amount);
		break;
	case EQuestType::EQT_GoToNPC:
		CheckProgressObjectType(correspondQuest, objectID);
		break;
	case EQuestType::EQT_GoToSpace:
		CheckProgressObjectType(correspondQuest, objectID);
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
	TArray<FQuest> resultArray;

	InProgressQuests.GenerateValueArray(resultArray);

	return resultArray;
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
	if (InProgressQuests.Find(questID))
	{
		return *(InProgressQuests.Find(questID));
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

void UCPP_QuestSubsystem::CheckProgressAmountType(FQuest& quest,int32 amount)
{
	if (quest.CurrentCount < quest.NeedCount)
	{
		quest.CurrentCount += amount;
	}
	else
	{
		return;
	}
	
	if (quest.NeedCount == quest.CurrentCount)
	{
		quest.QuestState = EQuestState::EQS_ConditionClear;

		OnChangeQuestInfo.Execute(quest);
	}
	else
	{
		quest.QuestState = EQuestState::EQS_InProgress;
		OnChangeQuestInfo.Execute(quest);
	}
}

void UCPP_QuestSubsystem::CheckProgressObjectType(FQuest& quest, const FName& objectID)
{
	if (quest.QuestState == EQuestState::EQS_ConditionClear)
	{
		return;
	}

	if (quest.NeedObjectID == objectID)
	{
		quest.QuestState = EQuestState::EQS_ConditionClear;
		OnChangeQuestInfo.Execute(quest);
	}
}
