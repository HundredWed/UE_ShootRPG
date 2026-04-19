// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ST_TotalPlayerData.h"
#include "CPP_SaveDataSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnDataLoadedDelegate, const FTotalSaveData&);

UCLASS()
class UE_RPG_API UCPP_SaveDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//플레이어
	const FCharacterStats& GetCharacterStatData() const { return TotalSaveData.CharacterStatData; }
	const FTransform& GetCharacterTransform() const { return TotalSaveData.PlayerTransform; }
	const TArray<FInventorySlot>& GetInventorySlotData() const { return TotalSaveData.SlotsArrayData; }
	const TMap<EEquipmentType, FEquipmentSlot>& GetEquipmentSlotData() const { return TotalSaveData.EquipmentSlotData; }

	void UpdateCharacterStatData(const FCharacterStats& newStats) { TotalSaveData.CharacterStatData = newStats; }
	void UpdateCharacterTransform(const FTransform& newTransform) { TotalSaveData.PlayerTransform = newTransform; }
	void UpdateInventorySlotData(const TArray<FInventorySlot>& newSlots) { TotalSaveData.SlotsArrayData = newSlots; }
	void UpdateEquipmentSlotData(const TMap<EEquipmentType, FEquipmentSlot>& newSlots) { TotalSaveData.EquipmentSlotData = newSlots; }


	//퀘스트
	const TMap<FName, FQuest>& GetInProgressQuestData() const { return TotalSaveData.InProgressQuests; }
	const TSet<FName>& GetClearQuestData() const { return TotalSaveData.ClearQuestData; }

	void UpdateInProgressQuestData(const TMap<FName, FQuest>& newData) { TotalSaveData.InProgressQuests = newData; }
	void UpdateClearQuestData(const TSet<FName>& newData) { TotalSaveData.ClearQuestData = newData; }

	void SaveGameData();
	void LoadGameData();

	FOnDataLoadedDelegate OnDataLoaded;

private:
	UPROPERTY()
	FTotalSaveData TotalSaveData;
};
