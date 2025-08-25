// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NonPlayerCharacterBase.h"
#include "Interface/CPP_InteractInterface.h"
#include "CPP_InteractiveNPC.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API ACPP_InteractiveNPC : public ANonPlayerCharacterBase , public ICPP_InteractInterface
{
	GENERATED_BODY()

public:

	virtual void Interact(AActor* Interactor) override;
	
};
