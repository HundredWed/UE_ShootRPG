#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Structs/ST_Quest.h"
#include "Structs/ST_Talk.h"
#include "Structs/ST_NPCQuests.h"
#include "CPP_DialogueSystem.generated.h"

class UCPP_CustomInstance;

UCLASS(Blueprintable, BlueprintType)
class UE_RPG_API UCPP_DialogueSystem : public UObject
{
	GENERATED_BODY()

public:
	UCPP_DialogueSystem();

	void AddProgressQuest(FQuest quest);
	void QuestClear(const FName& questID);
	FQuest CheckQuestContent(const FName& objectID, const int32 amount);

	//getter
	TArray<FQuest> GetQusetList(const FName& npcID);
	FTalkDialogue GetTalkStruct(const FName& npcID);
	FTalkDialogue GetQuestDialogueStruct(const FName& npcID);
	TArray<FQuest> GetProgrssQusetsOfPlayer();
	FQuest GetQuestInfo(const FName& npcID, FName questID);

private:

	FQuest CheckProgress(const FName& questID);
	bool CheckClear(const FName& questID);
	int32 CastQuestIndex(FName questID);

	void CheckProgressGetItmeType(FQuest& quest, const int32 amount, const int32 index);
	void CheckProgressGoToNPCType(FQuest& quest, const FName& npcID, const int32 index);
	void CheckProgressGoToSpaceType();
	void CheckProgressComBatType();

private:

	//DataTable
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* QuestDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* TalkDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* QuestDialogueDataTable;

	UPROPERTY()
	UCPP_CustomInstance* CustomGameInstance;

	//Player
	UPROPERTY()
	TArray<FQuest> InProgressQuests;

	//NPC
	/*UPROPERTY()
	TMap<FName, FQuest> AcceptedQuests;*/
	UPROPERTY()
	TSet<FName> ClearQuests;
	
};
