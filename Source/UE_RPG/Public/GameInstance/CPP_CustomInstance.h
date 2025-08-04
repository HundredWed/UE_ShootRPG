// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "CPP_CustomInstance.generated.h"

class UInputMappingContext;
class UCPP_DialogueSystem;

UCLASS()
class UE_RPG_API UCPP_CustomInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:


private:

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TArray<UInputMappingContext*> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UCPP_DialogueSystem* QuestSystem;
};
