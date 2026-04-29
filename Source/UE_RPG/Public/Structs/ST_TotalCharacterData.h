#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "Structs/ST_InventoryData.h"
#include "Structs/ST_Quest.h"
#include "ST_TotalCharacterData.generated.h"


USTRUCT(BlueprintType)
struct FTotalSaveData
{
    GENERATED_BODY()

public:

	//플레이어
	UPROPERTY()
	FCharacterStats CharacterStatData;
	UPROPERTY()
	FTransform PlayerTransform;

	//인벤토리
	UPROPERTY()
	FInventoryTotalData InventoryTotalData;

	//퀘스트
	UPROPERTY()
	TMap<FName, FQuest> InProgressQuests;
	UPROPERTY()
	TSet<FName> ClearQuestData;
};
