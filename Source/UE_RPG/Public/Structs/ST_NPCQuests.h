#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/ST_Quest.h"
#include "ST_NPCQuests.generated.h"

USTRUCT(BlueprintType)
struct FNPCQuests : public FTableRowBase
{
	GENERATED_BODY()

public:
	FNPCQuests() {};

	/**FName이 퀘스트 ID*/
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FQuest> Quests;
};
