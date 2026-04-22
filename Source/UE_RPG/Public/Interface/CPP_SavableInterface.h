// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_SavableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_SavableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_RPG_API ICPP_SavableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//데이터 세이브
	virtual void GatherSaveData(class UCPP_SaveDataSubsystem* saveSystem) = 0;

	//데이터 로드
	virtual void ApplySaveData(class UCPP_SaveDataSubsystem* SaveSubsystem) = 0;
};
