#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/ST_Quest.h"
#include "ST_NPCQuests.generated.h"

USTRUCT(BlueprintType)
struct FAnswerDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAnswerDialogue() {};

	UPROPERTY(EditDefaultsOnly)
	TArray<FQuest> Quests;
};
