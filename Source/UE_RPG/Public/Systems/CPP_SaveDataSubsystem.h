// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ST_TotalCharacterData.h"
#include "CPP_SaveDataSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnDataLoadedDelegate, const FTotalSaveData&);
DECLARE_MULTICAST_DELEGATE(FOnGatherSaveDataDelegate);

UCLASS()
class UE_RPG_API UCPP_SaveDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCPP_SaveDataSubsystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	//플레이어
	const FCharacterStats& GetCharacterStatData() const { return TotalSaveData.CharacterStatData; }
	const FTransform& GetCharacterTransform() const { return TotalSaveData.PlayerTransform; }
	const FInventoryTotalData& GetInventoryData() const { return TotalSaveData.InventoryTotalData; }

	void UpdateCharacterStatData(const FCharacterStats& newStats) { TotalSaveData.CharacterStatData = newStats; }
	void UpdateCharacterTransform(const FTransform& newTransform) { TotalSaveData.PlayerTransform = newTransform; }
	void UpdateInventoryData(const FInventoryTotalData& data) { TotalSaveData.InventoryTotalData = data; }


	//퀘스트
	const TMap<FName, FQuest>& GetInProgressQuestData() const { return TotalSaveData.InProgressQuests; }
	const TSet<FName>& GetClearQuestData() const { return TotalSaveData.ClearQuestData; }

	void UpdateInProgressQuestData(const TMap<FName, FQuest>& newData) { TotalSaveData.InProgressQuests = newData; }
	void UpdateClearQuestData(const TSet<FName>& newData) { TotalSaveData.ClearQuestData = newData; }

	void SaveGameData();
	void SaveGameDataAsync();
	void SaveAndQuitGame();
	void LoadGameData();


	FOnGatherSaveDataDelegate OnGatherSaveData;
	FOnDataLoadedDelegate OnDataLoaded;

private:

	void DefaultSaveSetting();
	FString SwitchSlot();
	void SaveMetaData(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	bool TryLoadFromSlot(const FString& targetSlot);

private:

	UPROPERTY()
	FTotalSaveData TotalSaveData;

	FTimerHandle AutoSaveTimerHandle;

	FString CurrentSlotName = "";
};
