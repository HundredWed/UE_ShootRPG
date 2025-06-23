#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enum/QuestEnum.h"
#include "ST_Quest.generated.h"

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

	UPROPERTY(EditDefaultsOnly)
	FName DialogueID;

	UPROPERTY(EditDefaultsOnly)
	FName NeedObjectID;

	UPROPERTY(EditDefaultsOnly)
	FName ChainQuestID;

	UPROPERTY(EditDefaultsOnly)
	FString QuestName;

	UPROPERTY(EditDefaultsOnly)
	FString QuestContent;

	UPROPERTY(EditDefaultsOnly)
	FString NeedObjectName;

	UPROPERTY(EditDefaultsOnly)
	int32 NeedCount;

	UPROPERTY(EditDefaultsOnly)
	float EXP;

	UPROPERTY(EditDefaultsOnly)
	EQusetType QusetType;

	UPROPERTY(EditDefaultsOnly)
	EQusetState QusetState;

	UPROPERTY(EditDefaultsOnly)
	UItem* QusetState;
};
