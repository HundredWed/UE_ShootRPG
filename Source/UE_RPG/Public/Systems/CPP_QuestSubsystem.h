// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ST_Quest.h"
#include "Structs/ST_NPCQuests.h"
#include "CPP_QuestSubsystem.generated.h"


DECLARE_DELEGATE_OneParam(FOnChangeQuestInfoDelegate, const FQuest&);

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

	FQuest CheckQuestContent(const FName& objectID, int32 amount = 0);

	TArray<FQuest> GetQuestList();
	TArray<FQuest> GetProgressQuestsOfPlayer();
	FQuest GetQuestInfo(const FName& questID);

	void SelectedQuest(const FName& questId);

	FOnChangeQuestInfoDelegate OnChangeQuestInfo;

private:

	FQuest CheckProgress(const FName& questID);
	bool CheckClear(const FName& questID);
	int32 CastQuestIndex(FName questID);

	void CheckProgressAmountType(FQuest& quest,int32 amount);
	void CheckProgressObjectType(FQuest& quest, const FName& objectID);
	
private:

	FQuest CurrentQuest;
	FName CurrentNPCID;

	//DataTable
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* QuestDataTable;

	//Player
	UPROPERTY()
	TMap<FName, FQuest> InProgressQuests;

	//NPC
	/*UPROPERTY()
	TMap<FName, FQuest> AcceptedQuests;*/
	UPROPERTY()
	TSet<FName> ClearQuests;
};
