#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_Quest.generated.h"

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	//카운팅
	EQT_GetItem UMETA(DisplayName = "GetItem"),
	EQT_ComBat UMETA(DisplayName = "ComBat"),

	//논 카운팅
	EQT_GoToNPC UMETA(DisplayName = "GoToNPC"),
	EQT_GoToSpace UMETA(DisplayName = "GoToSpace")

};

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	EQS_Normal UMETA(DisplayName = "Normal"),
	EQS_InProgress UMETA(DisplayName = "InProgress"),
	EQS_ConditionClear UMETA(DisplayName = "ConditionClear"),
	EQS_Clear UMETA(DisplayName = "Clear")
};

class UItem;

USTRUCT(BlueprintType)
struct FQuest : public FTableRowBase
{
	GENERATED_BODY()

public:
	FQuest() {};

	UPROPERTY(EditDefaultsOnly)
	FName QuestID;

	UPROPERTY(EditDefaultsOnly)
	FName PrevQuestID;

	////현재 사용x
	//UPROPERTY(VisibleAnywhere)
	//FName DialogueID;

	UPROPERTY(EditDefaultsOnly)
	FName QuestProgressRow;

	UPROPERTY(EditDefaultsOnly)
	FName QuestClearRow;
	
	UPROPERTY(EditDefaultsOnly)
	FName NeedContentID;

	UPROPERTY(EditDefaultsOnly)
	FName ChainQuestID;

	UPROPERTY(EditDefaultsOnly)
	FText QuestName;

	UPROPERTY(EditDefaultsOnly)
	FText QuestContent;

	UPROPERTY(EditDefaultsOnly)
	FText NeedObjectName;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentCount;

	UPROPERTY(EditDefaultsOnly)
	int32 NeedCount;

	UPROPERTY(EditDefaultsOnly)
	float EXP;

	UPROPERTY(EditDefaultsOnly)
	EQuestType QuestType;

	UPROPERTY(EditDefaultsOnly)
	EQuestState QuestState;

	UPROPERTY(EditDefaultsOnly)
	FName RewardItemID;
	UPROPERTY(EditDefaultsOnly)
	int32 RewardItemAmount = 1;
};
