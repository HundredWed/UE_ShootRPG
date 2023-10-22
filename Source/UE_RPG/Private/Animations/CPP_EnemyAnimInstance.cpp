// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CPP_EnemyAnimInstance.h"
#include "NPC/EnemyBase.h"

void UCPP_EnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EnemyPawn = Cast<AEnemyBase>(TryGetPawnOwner());
}


