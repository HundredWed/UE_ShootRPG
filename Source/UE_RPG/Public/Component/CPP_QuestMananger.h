// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/ST_Quest.h"
#include "Structs/ST_NPCQuests.h"
#include "CPP_QuestMananger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_QuestMananger : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_QuestMananger();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddProgressQuest();
	void QuestClear();
	FQuest CheckQuestContent(const FName& objectID, const int32 amount);

	TArray<FQuest> GetQusetList(const FName& npcID);
	TArray<FQuest> GetProgrssQusetsOfPlayer();
	FQuest GetQuestInfo(const FName& npcID, FName questID);

	FQuest SelectedQuest(const FName& npcID, const FName& questId);


protected:
	virtual void BeginPlay() override;

private:
	
	FQuest CheckProgress(const FName& questID);
	bool CheckClear(const FName& questID);
	int32 CastQuestIndex(FName questID);

	void CheckProgressGetItmeType(FQuest& quest, const int32 amount, const int32 index);
	void CheckProgressGoToNPCType(FQuest& quest, const FName& npcID, const int32 index);
	void CheckProgressGoToSpaceType();
	void CheckProgressComBatType();

private:

	FQuest CurrentQuest;

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
