// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CPP_SaveDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CPP_SaveGame.h"
#include "Systems/SystemSaveGame.h"

UCPP_SaveDataSubsystem::UCPP_SaveDataSubsystem()
{
}

void UCPP_SaveDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UGameplayStatics::DoesSaveGameExist(TEXT("SystemData"), 0))
	{
		if (USystemSaveGame* systemData = Cast<USystemSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SystemData"), 0)))
		{
			CurrentSlotName = systemData->LatestSaveSlotName;
		}

		if (!TryLoadFromSlot(CurrentSlotName))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s :세이브 파일이 없거나 유효하지 않습니다. 다른 슬롯의 로드를 시도합니다."), *CurrentSlotName);

			CurrentSlotName = SwitchSlot();
			if (!TryLoadFromSlot(CurrentSlotName))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s : 유효한 슬롯이 없습니다. 디폴트 데이터로 설정 됩니다."), *CurrentSlotName);
				DefaultSaveSetting();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("최근 세이브 파일이 없습니다. 디폴트 데이터로 설정 됩니다."));
		DefaultSaveSetting();		
	}

	
}

void UCPP_SaveDataSubsystem::SaveGameData()
{
	OnGatherSaveData.Broadcast();

	UCPP_SaveGame* saveObject = Cast<UCPP_SaveGame>(UGameplayStatics::CreateSaveGameObject(UCPP_SaveGame::StaticClass()));

	if (saveObject)
	{
		CurrentSlotName = SwitchSlot();

		saveObject->TotalSaveData = TotalSaveData;
		saveObject->SlotName = CurrentSlotName;

		if (UGameplayStatics::SaveGameToSlot(saveObject, CurrentSlotName, 0))
		{
			UE_LOG(LogTemp, Log, TEXT("데이터 저장 성공!"));

			SaveMetaData(CurrentSlotName, 0, true);
		}
	}
}

void UCPP_SaveDataSubsystem::SaveGameDataAsync()
{
	OnGatherSaveData.Broadcast();

	UCPP_SaveGame* saveObject = Cast<UCPP_SaveGame>(UGameplayStatics::CreateSaveGameObject(UCPP_SaveGame::StaticClass()));

	if (saveObject)
	{
		CurrentSlotName = SwitchSlot();

		saveObject->TotalSaveData = TotalSaveData;
		saveObject->SlotName = CurrentSlotName;

		FAsyncSaveGameToSlotDelegate savedDelegate;
		savedDelegate.BindUObject(this, &UCPP_SaveDataSubsystem::SaveMetaData);
		UGameplayStatics::AsyncSaveGameToSlot(saveObject, CurrentSlotName, 0, savedDelegate);
	}
}

void UCPP_SaveDataSubsystem::LoadGameData()
{
	if (TryLoadFromSlot(CurrentSlotName))
	{
		if (OnDataLoaded.IsBound())
		{
			OnDataLoaded.Broadcast(TotalSaveData);
		}
	}
	else
	{
		DefaultSaveSetting();
		UE_LOG(LogTemp, Error, TEXT("세이브 파일이 없거나 유효하지 않습니다. 디폴트 데이터로 설정 됩니다."));
	}
}

void UCPP_SaveDataSubsystem::DefaultSaveSetting()
{
	FTotalSaveData defaultTotalSaveData;
	TotalSaveData = defaultTotalSaveData;

	CurrentSlotName = TEXT("Slot_A");

	USystemSaveGame* systemSaveObject = Cast<USystemSaveGame>(UGameplayStatics::CreateSaveGameObject(USystemSaveGame::StaticClass()));
	if (systemSaveObject)
	{
		systemSaveObject->LatestSaveSlotName = CurrentSlotName;
		UGameplayStatics::SaveGameToSlot(systemSaveObject, TEXT("SystemData"), 0);
	}
}

FString UCPP_SaveDataSubsystem::SwitchSlot()
{
	UE_LOG(LogTemp, Log, TEXT("슬롯 교체"));
	return (CurrentSlotName == TEXT("Slot_A")) ? TEXT("Slot_B") : TEXT("Slot_A");
}

void UCPP_SaveDataSubsystem::SaveMetaData(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess)
	{
		USystemSaveGame* systemSaveObject = Cast<USystemSaveGame>(UGameplayStatics::CreateSaveGameObject(USystemSaveGame::StaticClass()));
		if (systemSaveObject)
		{
			systemSaveObject->LatestSaveSlotName = SlotName;
			UGameplayStatics::SaveGameToSlot(systemSaveObject, TEXT("SystemData"), 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SystemData 갱신에 실패 했습니다."));
	}
}

bool UCPP_SaveDataSubsystem::TryLoadFromSlot(const FString& targetSlot)
{
	if (UGameplayStatics::DoesSaveGameExist(targetSlot, 0))
	{
		UCPP_SaveGame* loadedData = Cast<UCPP_SaveGame>(UGameplayStatics::LoadGameFromSlot(targetSlot, 0));

		if (loadedData)
		{
			TotalSaveData = loadedData->TotalSaveData;

			UE_LOG(LogTemp, Log, TEXT("%s : 세이브 데이터 로드 성공"), *targetSlot);

			return true;
		}
	}

	return false;
}
