// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animations/CPP_NPCAnimInstance.h"
#include "AIController.h"

#include "CPP_Character.h"

#define NO_TARGET -1

AEnemyBase::AEnemyBase()
{

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HitActionMontage_NoDamaged) && !IsValid(HitActionMontage))
	{
		WARNINGLOG(TEXT("Enemy HitMontage is not Valid!!"));
	}

	EnemyController = Cast<AAIController>(GetController());
	if (!IsValid(EnemyController))
	{
		WARNINGLOG(TEXT("Enemy EnemyController is not Valid!!"));
		return;
	}

	CharaterType = ECharacterTypes::NPC_Monster;
	SpawnPos = GetActorLocation();
	SetActorTickEnabled(false);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NPCState == ENPCState::Chase)
	{
		ThinkAction();
	}
}

void AEnemyBase::SetActionStateNormal()
{
	ENPCActionState = ENPCActionState::Normal;
}

void AEnemyBase::UpdateState()
{	
	if (NPCState == ENPCState::Death)
	{
		BehaviorMode(NPCState = ENPCState::Death);
	}
	else if (!IsValid(Target))
	{
		ClearTargetInfo();
		BehaviorMode(NPCState = ENPCState::Patrol);
	}
	else if (IsValid(Target))
	{
		float dis = CheckDist();
		if (dis < 0)
			return;

		if (dis > CombatDis)
		{
			BehaviorMode(NPCState = ENPCState::Chase);
		}
		else
		{
			BehaviorMode(NPCState = ENPCState::Combat);
		}
	}
}

bool AEnemyBase::GetHit(const FVector& targetLocation)
{
	if (NPCState == ENPCState::Death)
		return false;

	if (!IsValid(NPCAnimInstance))
	{
		WARNINGLOG(TEXT("is not IsValid Enemy AnimInstance!!"));
		return false;
	}

	float dis = (GetActorLocation() - targetLocation).Length();

	if (dis >= NoDamagedDistance)
	{
		NoDamaged(targetLocation);
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Black, TEXT("NoDamaged!!"));
		return false;
	}

	NPCAnimInstance->Montage_Play(HitActionMontage);
	return true;
}

void AEnemyBase::NoDamaged(const FVector& targetLocation)
{
	SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Ouch!!"));
	NPCAnimInstance->Montage_Play(HitActionMontage_NoDamaged);
}

void AEnemyBase::BehaviorMode(ENPCState enemyState)
{
	FTimerHandle TimerHandle;
	float animLength = CombatActionMontage->GetPlayLength() + 0.05f;
	switch (enemyState)
	{
	case ENPCState::Patrol:
		Patrol();
		SetHealthBarWidget(true);
		bOrderfromSpawnArea = false;
		break;
	case ENPCState::Combat:
		NPCAnimInstance->Montage_Play(CombatActionMontage);
		EnemyController->StopMovement();
		LookAtTarget();
		if (CanUpdateState())
		{
			// юс╫ц
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::UpdateState, animLength, false);
		}
		break;
	case ENPCState::Chase:
		SetHealthBarWidget(true);
		ChaseTarget();
		break;
	case ENPCState::Death:
		EnemyController->StopMovement();
		SetActorTickEnabled(false);
		break;
	default:
		break;
	}
}


void AEnemyBase::Spawn()
{
	SetActorHiddenInGame(false);
}

void AEnemyBase::UnSpawn()
{
	SetActorHiddenInGame(true);
}

float AEnemyBase::CheckDist()
{
	if (!IsValid(Target))
		return NO_TARGET;

	FVector targetPos = Target->GetActorLocation();
	return (targetPos - GetActorLocation()).Length();
}

bool AEnemyBase::CanUpdateState()
{
	return !bOrderfromSpawnArea && 
		ENPCActionState != ENPCActionState::Action &&
		NPCState != ENPCState::Death;
}

void AEnemyBase::SetTarget(ACPP_Character* target)
{
	Target = target;
	UpdateState();
}

void AEnemyBase::Patrol()
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(SpawnPos);
	MoveRequest.SetAcceptanceRadius(3.f);
	EnemyController->MoveTo(MoveRequest);

	if (CurrentHP != MaxHealth)
		CurrentHP = MaxHealth;

	UpdateHealthPercent(CurrentHP);
}

void AEnemyBase::ChaseTarget()
{
	if (!IsValid(Target))
		return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(3.f);
	EnemyController->MoveTo(MoveRequest);

	SetActorTickEnabled(true);
}

void AEnemyBase::ThinkAction()
{
	float dis = CheckDist();

	if (dis < 0)
		return;

	if (dis < CombatDis)
	{
		SetActorTickEnabled(false);
		UpdateState();
	}
}
