// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "CPP_MyGameSettings.generated.h"

class APickUpItem;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "My RPG Settings"))
class UE_RPG_API UCPP_MyGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:


	//Dialogue
    UPROPERTY(Config, EditAnywhere, Category = "Dialogue Data")
    TSoftObjectPtr<UDataTable> TalkData;

    UPROPERTY(Config, EditAnywhere, Category = "Dialogue Data")
    TSoftObjectPtr<UDataTable> QuestDialogueData;

	UPROPERTY(Config, EditAnywhere, Category = "Dialogue Data")
	TSoftObjectPtr<UDataTable> NPCData;

	//Quest
	UPROPERTY(Config, EditAnywhere, Category = "Quest Data")
	TSoftObjectPtr<UDataTable> QuestData;

	//Item
	UPROPERTY(Config, EditAnywhere, Category = "Item Data")
	TSoftObjectPtr<UDataTable> ItemData;

	UPROPERTY(Config, EditAnywhere, Category = "Item Data")
	TSoftObjectPtr<UDataTable> WeaponData;
	

	UPROPERTY(Config, EditAnywhere, Category = "Drop Item Settings")
	TSoftClassPtr<AActor> EquipmentDropClass;
	UPROPERTY(Config, EditAnywhere, Category = "Drop Item Settings")
	TSoftClassPtr<AActor> ItemDropClass;

public:

	static UDataTable* LoadDataTableSafely(const TSoftObjectPtr<UDataTable>& SoftTablePtr);
	static UClass* LoadWorldObjSubClassSafely(const TSoftClassPtr<AActor>& SoftTablePtr);

};
