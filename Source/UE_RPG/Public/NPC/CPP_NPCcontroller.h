// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_NPCcontroller.generated.h"

class ANonPlayerCharacterBase;

UCLASS()
class UE_RPG_API ACPP_NPCcontroller : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void UpdateState();

	FORCEINLINE void SetControlOwner(ANonPlayerCharacterBase* owner) { NPC = owner; }
	FORCEINLINE void CanUpdateState(bool bupdate) { bUpdateState = bupdate; }

	
private:
	
	ANonPlayerCharacterBase* NPC;
	bool bUpdateState;
};
