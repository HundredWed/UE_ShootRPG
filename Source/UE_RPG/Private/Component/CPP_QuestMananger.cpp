#include "Component/CPP_QuestMananger.h"
#include "Kismet/KismetStringLibrary.h"

UCPP_QuestMananger::UCPP_QuestMananger()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCPP_QuestMananger::AddProgressQuest()
{
	if (CurrentQuest.QuestID.IsNone())
	{
		return;
	}

	CurrentQuest.QusetState = EQuestState::EQS_InProgress;

	InProgressQuests.Add(CurrentQuest);
	//AcceptedQuests.Add(quest.QuestID, quest);
}

void UCPP_QuestMananger::QuestClear()
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

FQuest UCPP_QuestMananger::CheckQuestContent(const FName& objectID, const int32 amount)
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

TArray<FQuest> UCPP_QuestMananger::GetQusetList(const FName& npcID)
{
	TArray<FQuest> qusetlists;

	FNPCQuests* qusets = QuestDataTable->FindRow<FNPCQuests>(npcID, "");

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

TArray<FQuest> UCPP_QuestMananger::GetProgrssQusetsOfPlayer()
{
	return InProgressQuests;
}

FQuest UCPP_QuestMananger::GetQuestInfo(const FName& npcID, FName questID)
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

FQuest UCPP_QuestMananger::SelectedQuest(const FName& npcID, const FName& rowName)
{
	CurrentQuest = GetQuestInfo(npcID, rowName);

	return CurrentQuest;
}

void UCPP_QuestMananger::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FQuest UCPP_QuestMananger::CheckProgress(const FName& questID)
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

bool UCPP_QuestMananger::CheckClear(const FName& questID)
{
	for (FName id : ClearQuests)
	{
		if (id == questID)
			return true;
	}
	return false;
}

int32 UCPP_QuestMananger::CastQuestIndex(FName questID)
{
	FString rowNameToString = questID.ToString();

	int32 index = UKismetStringLibrary::FindSubstring(rowNameToString, "N");

	FString questIndex = UKismetStringLibrary::GetSubstring(rowNameToString, 0, index);

	return FCString::Atoi(*questIndex);
}

void UCPP_QuestMananger::CheckProgressGetItmeType(FQuest& quest, const int32 amount, const int32 index)
{
	if (quest.NeedCount == amount)
	{
		InProgressQuests[index].QusetState = EQuestState::EQS_ConditionClear;
		quest = InProgressQuests[index];
	}
}

void UCPP_QuestMananger::CheckProgressGoToNPCType(FQuest& quest, const FName& npcID, const int32 index)
{
	if (quest.NeedObjectID == npcID)
	{
		InProgressQuests[index].QusetState = EQuestState::EQS_ConditionClear;
		quest = InProgressQuests[index];
	}
}

void UCPP_QuestMananger::CheckProgressGoToSpaceType()
{
}

void UCPP_QuestMananger::CheckProgressComBatType()
{
}

