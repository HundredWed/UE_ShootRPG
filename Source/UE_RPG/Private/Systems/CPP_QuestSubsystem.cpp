#include "Systems/CPP_QuestSubsystem.h"
#include "Kismet/KismetStringLibrary.h"
#include "Systems/CPP_MyGameSettings.h"

UCPP_QuestSubsystem::UCPP_QuestSubsystem()
{
}

void UCPP_QuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UCPP_MyGameSettings* Settings = GetDefault<UCPP_MyGameSettings>();

	QuestDataTable = UCPP_MyGameSettings::LoadDataTableSafely(Settings->QuestData);
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
	OnAddProgress.Broadcast(CurrentQuest);
}

void UCPP_QuestSubsystem::QuestClear()
{
	ClearQuests.Add(CurrentQuest.QuestID);
	InProgressQuests.Remove(CurrentQuest.QuestID);
	OnQuestClear.Broadcast(CurrentQuest);
	DISPLAYLOG(TEXT("%s: 퀘스클리어!"), *CurrentQuest.QuestName.ToString());
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

void UCPP_QuestSubsystem::CheckQuestContent(const FName& objectID, int32 amount)
{
	//해당 ID를 필요로 하는 퀘스트가 있는지 확인
	//있다면 진행도 확인

	if (!objectID.IsValid())
	{
		WARNINGLOG(TEXT("UCPP_QuestSubsystem::CheckQuestContent : 유효하지 않은 오브젝트 ID 입니다!"))
		return;
	}

	FQuest correspondQuest;
	for (auto& quest : InProgressQuests)
	{
		FQuest& targetQuest = quest.Value;

		if (targetQuest.NeedContentID == objectID)
		{
			switch (targetQuest.QuestType)
			{
			case EQuestType::EQT_GetItem:
				CheckProgressAmountType(targetQuest, amount);
			case EQuestType::EQT_ComBat:
				CheckProgressAmountType(targetQuest, amount);
				break;
			case EQuestType::EQT_GoToNPC:
				CheckProgressObjectType(targetQuest, objectID);
			case EQuestType::EQT_GoToSpace:
				CheckProgressObjectType(targetQuest, objectID);
				break;
			}
		}
	}
}

TArray<FQuest> UCPP_QuestSubsystem::GetQuestList()
{
	FNPCQuests* quests = QuestDataTable->FindRow<FNPCQuests>(CurrentNPCID, "");

	if (quests == nullptr)
		return TArray<FQuest>();

	TArray<FQuest> questLists;
	questLists.Reserve(quests->Quests.Num());

	for (const auto& quest : quests->Quests)
	{
		if (CheckClear(quest.Value.QuestID))
			continue;

		//선행퀘스트 완료 유무
		if (!quest.Value.PrevQuestID.IsNone() && CheckClear(quest.Value.PrevQuestID))
		{
			questLists.Add(CheckProgress(quest.Value.QuestID));
			continue;
		}

		FQuest progressQuest = CheckProgress(quest.Value.QuestID);

		if (progressQuest.QuestID.IsNone())
		{
			questLists.Add(quest.Value);
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
	FNPCQuests* quests = QuestDataTable->FindRow<FNPCQuests>(CurrentNPCID, "");

	if (quests)
	{
		return *(quests->Quests.Find(questID));
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
	return ClearQuests.Contains(questID);
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
	quest.CurrentCount = FMath::Min(quest.CurrentCount + amount, quest.NeedCount);

	if (quest.NeedCount == quest.CurrentCount)
	{
		quest.QuestState = EQuestState::EQS_ConditionClear;
	}
	else
	{
		quest.QuestState = EQuestState::EQS_InProgress;
	}

	OnChangeQuestInfo.Execute(quest);
}

void UCPP_QuestSubsystem::CheckProgressObjectType(FQuest& quest, const FName& objectID)
{
	if (quest.QuestState == EQuestState::EQS_ConditionClear)
	{
		return;
	}

	if (quest.NeedContentID == objectID)
	{
		quest.QuestState = EQuestState::EQS_ConditionClear;
		OnChangeQuestInfo.Execute(quest);
	}
}
