// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterStates.h"
#include "CPP_InteractInterface.generated.h"

class AActor;

UINTERFACE(MinimalAPI)
class UCPP_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_RPG_API ICPP_InteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void RequestInteract(AActor* Interactor) {}
	virtual void EndInteract() {}

	virtual void OnBeginLookAt() {}
	virtual void OnEndLookAt() {}

	virtual ECharacterTypes GetType() { return  ECharacterTypes::Type_None; }
};
