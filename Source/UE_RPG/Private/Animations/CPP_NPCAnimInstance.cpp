// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CPP_NPCAnimInstance.h"
#include "NPC/EnemyBase.h"

void UCPP_NPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NPCPawn = Cast<AEnemyBase>(TryGetPawnOwner());
}

void UCPP_NPCAnimInstance::SetNPCActionState(ECharacterActionState state)
{
	NPCActionState = state;
}


