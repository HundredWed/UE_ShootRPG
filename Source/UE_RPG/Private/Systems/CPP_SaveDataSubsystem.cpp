// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/CPP_SaveDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/CPP_SaveGame.h"

void UCPP_SaveDataSubsystem::SaveGameData()
{
	UCPP_SaveGame* saveObject = Cast<UCPP_SaveGame>(UGameplayStatics::CreateSaveGameObject(UCPP_SaveGame::StaticClass()));

	if (saveObject)
	{
		saveObject->TotalSaveData = TotalSaveData;

		if (UGameplayStatics::SaveGameToSlot(saveObject, TEXT("Slot1"), 0))
		{
			UE_LOG(LogTemp, Log, TEXT("데이터 세이브 성공!"));
		}
	}
}

void UCPP_SaveDataSubsystem::LoadGameData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("Slot1"), 0))
	{
		UCPP_SaveGame* loadedData = Cast<UCPP_SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"), 0));

		if (loadedData)
		{
			TotalSaveData = loadedData->TotalSaveData;

			//UE_LOG(LogTemp, Log, TEXT("로드 성공! 레벨: %d"), RestoredLevel);

			//방송
			if (OnDataLoaded.IsBound())
			{
				OnDataLoaded.Broadcast(TotalSaveData);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("세이브 파일이 없습니다. 디폴트 데이터로 설정 됩니다."));
	}
}
