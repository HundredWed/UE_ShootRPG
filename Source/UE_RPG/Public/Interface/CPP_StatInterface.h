// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStates.h"
#include "CPP_StatInterface.generated.h"


UINTERFACE(MinimalAPI)
class UCPP_StatInterface : public UInterface
{
	GENERATED_BODY()
};


class UE_RPG_API ICPP_StatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnRestore(ERestoreTypes restoreTypes, const int32) {};
};
