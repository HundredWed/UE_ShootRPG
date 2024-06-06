// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/CPP_NPCcontroller.h"
#include "NPC/NonPlayerCharacterBase.h"

void ACPP_NPCcontroller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (bUpdateState)
		UpdateState();
}

void ACPP_NPCcontroller::UpdateState()
{
	if (!IsValid(NPC))
		return;
	NPC->UpdateState();
	NPC->bTurningLoop = false;
	bUpdateState = false;
}


