// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ST_Quest.h"
#include "Structs/ST_NPCQuests.h"
#include "CPP_QuestSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_QuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCPP_QuestSubsystem();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitQuestSubsystem(const FName& currentNpcID);

	void AddProgressQuest();

	void QuestClear();
	void QuestStateChange(EQuestState questState);

	FQuest CheckQuestContent(const FName& objectID, const int32 amount);

	TArray<FQuest> GetQuestList();
	TArray<FQuest> GetProgressQuestsOfPlayer();
	FQuest GetQuestInfo(const FName& questID);

	void SelectedQuest(const FName& questId);


private:

	FQuest CheckProgress(const FName& questID);
	bool CheckClear(const FName& questID);
	int32 CastQuestIndex(FName questID);

	void CheckProgressGetItemType(FQuest& quest, const int32 amount, const int32 index);
	void CheckProgressGoToNPCType(FQuest& quest, const int32 index);
	void CheckProgressGoToSpaceType();
	void CheckProgressComBatType();

private:

	FQuest CurrentQuest;
	FName CurrentNPCID;

	//DataTable
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* QuestDataTable;

	//Player
	UPROPERTY()
	TArray<FQuest> InProgressQuests;

	//NPC
	/*UPROPERTY()
	TMap<FName, FQuest> AcceptedQuests;*/
	UPROPERTY()
	TSet<FName> ClearQuests;
};
