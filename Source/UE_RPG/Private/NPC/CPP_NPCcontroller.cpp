// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/CPP_NPCcontroller.h"
#include "NPC/EnemyBase.h"

void ACPP_NPCcontroller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (bUpdateState)
		UpdateState();
}

void ACPP_NPCcontroller::UpdateState()
{
	if (!IsValid(Enemy))
		return;
	Enemy->UpdateState();
	Enemy->bTurningLoop = false;
	bUpdateState = false;
}


