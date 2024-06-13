// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "CPP_Character.h"
#include "NPC/CPP_EnemyCombatBox.h"
#include "NPC/CPP_EnemySpawnArea.h"
#include "Animations/CPP_NPCAnimInstance.h"

#define NO_TARGET 0

AEnemyBase::AEnemyBase()
{
	CorwdTraceRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HitActionMontage_NoDamaged) && !IsValid(HitActionMontage))
	{
		WARNINGLOG(TEXT("Enemy HitMontage is not Valid!!"));
	}

	if (CombatTypes == EEnemyCombatTypes::Dummy || !IsValid(CombatBoxClass))
	{
		WARNINGLOG(TEXT("Please set CombatType!!"));
	}

	InitEnenmyInfo();
	CharaterType = ECharacterTypes::NPC_Monster;
	WeaponReady();
	SpawnPos = GetActorLocation();
	SetControlOwner(this);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ThinkAction();
}

void AEnemyBase::SetActionStateNormal()
{
	ENPCActionState = ENPCActionState::Normal;
}

void AEnemyBase::UpdateState()
{	
	Super::UpdateState();

	if (NPCState == ENPCState::Death)
	{
		BehaviorMode(NPCState = ENPCState::Death);
	}
	else if (!IsValid(Target))
	{
		BehaviorMode(NPCState = ENPCState::Patrol);
	}
	else
	{	
		float dis = CheckDist();
		if (dis < NO_TARGET)
			return;

		if (bCorwd)
		{
			BehaviorMode(NPCState = ENPCState::SideStep);
		}
		else if (dis > CombatDis)
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

	float dis = (GetActorLocation() - targetLocation).Length();

	if (dis >= NoDamagedDistance)
	{
		NoDamaged(targetLocation);
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Black, TEXT("NoDamaged!!"));
		return false;
	}

	PlayNPCMontage(HitActionMontage);
	return true;
}

void AEnemyBase::NoDamaged(const FVector& targetLocation)
{
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Ouch!!"));
	PlayNPCMontage(HitActionMontage_NoDamaged);
}

void AEnemyBase::ThinkAction()
{
	float dis = CheckDist();

	if (dis < NO_TARGET)
		return;

	if (dis < ValidSightDis && IsCorwd())
	{
		SetActorTickEnabled(false);
		bCorwd = true;
		UpdateState();
	}
		
	if (dis < CombatDis)
	{
		SetActorTickEnabled(false);
		UpdateState();
	}
}

void AEnemyBase::BehaviorMode(ENPCState enemyState)
{
	InitBehaviorState();

	switch (enemyState)
	{
	case ENPCState::Patrol:
		ClearTargetInfo();
		Patrol();
		break;
	case ENPCState::Combat:
		Combat();
		break;
	case ENPCState::Chase:
		ChaseTarget();
		SetHealthBarWidget(true);
		SetActorTickEnabled(true);
		break;
	case ENPCState::SideStep:
		LookAtTarget(Target->GetActorLocation());
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::SideStep, DELAY4, false);
		break;
	case ENPCState::Death:
		MySpawnArea->EnemyDeathCount();
		break;
	default:
		break;
	}
}

void AEnemyBase::InitBehaviorState()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = DelfaultSpeed;
	SetActorTickEnabled(false);
	bCorwd = false;
	StopMove();
}

void AEnemyBase::Spawn(ACPP_EnemySpawnArea* spawnarea)
{
	MySpawnArea = spawnarea;
	SetActorHiddenInGame(false);
}

void AEnemyBase::UnSpawn()
{
	SetActorHiddenInGame(true);
}

bool AEnemyBase::CanUpdateState()
{
	return !bOrderfromSpawnArea && 
		ENPCActionState != ENPCActionState::Action &&
		NPCState != ENPCState::Death;
}

void AEnemyBase::SetTarget(ACPP_Character* target)
{
	if (!IsValid(target))
		return;

	Target = target;
	UpdateState();
}

void AEnemyBase::WeaponReady()
{
	UWorld* World = GetWorld();
	if (!IsValid(CombatBoxClass) && !IsValid(World))
		return;

	uint8 weapons = (uint8)CombatTypes;

	if (weapons != SocketNames.Num())
		return;

	for (uint8 i = 0; i < weapons; i++)
	{
		ACPP_EnemyCombatBox* weapon = World->SpawnActor<ACPP_EnemyCombatBox>(CombatBoxClass);
		weapon->SetDamage(ATK);
		CombatBoxes.Push(weapon);
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		CombatBoxes[i]->AttachToComponent(GetMesh(), TransformRules, SocketNames[i]);
	}
}

void AEnemyBase::InitEnenmyInfo()
{
	if (IsValid(EnemyDataTable))
	{
		const FEnemyInfoTable* info = EnemyDataTable->FindRow<FEnemyInfoTable>(EnemyID,TEXT(""));
		if (info == nullptr)
		{
			WARNINGLOG(TEXT("[%s] was not found!!Please check the ID."), *EnemyID.ToString())
				return;
		}

		CombatTypes = info->CombatTypes;
		PlaySection = info->PlaySection;
		SocketNames = info->SocketNames;
		NoDamagedDistance = info->NoDamagedDistance;
		CombatDis = info->CombatDis;
		ValidSightDis = info->ValidSightDis;
	}
}

void AEnemyBase::Patrol()
{
	SetHealthBarWidget(true);
	bOrderfromSpawnArea = false;

	MoveToLocation(SpawnPos);
	SetHPMAX();
}

void AEnemyBase::SideStep()
{
	GetCharacterMovement()->MaxWalkSpeed = SidStepSpeed;
	FVector rightVector = GetActorRightVector().GetSafeNormal();

	int32 randomDir = FMath::RandRange(-1, 0);
	float dir = randomDir == -1 ? -1.f : 1.f;
	NPCAnimInstance->Angle = dir;

	FVector sideVector = GetActorLocation() + (rightVector * dir * 300.f);

	MoveSide(sideVector);
}

void AEnemyBase::Combat()
{
	float animLength;

	if (PlaySection.Num() > 0)
	{
		int32 random = FMath::RandRange(0, PlaySection.Num());
		if (random == PlaySection.Num())
		{
			BehaviorMode(NPCState = ENPCState::SideStep);
			return;
		}
		else
			animLength = PlayNPCMontage(CombatActionMontage, PlaySection[random]);
	}
	else
	{
		animLength = PlayNPCMontage(CombatActionMontage);
	}

	LookAtTarget(Target->GetActorLocation());

	if (CanUpdateState() && animLength > 0)
	{
		// юс╫ц
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::UpdateState, animLength, false);
	}
}

bool AEnemyBase::IsCorwd()
{
	//WARNINGLOG(TEXT("IsCorwd"))
	FHitResult hit;
	FVector start = GetActorLocation();
	FVector end = Target->GetActorLocation();

	FCollisionShape sphere = FCollisionShape::MakeSphere(CorwdTraceRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool ishit = GetWorld()->SweepSingleByChannel(
		hit,
		start,
		end,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		sphere,
		Params);

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 5.f);

	AEnemyBase* actor = Cast<AEnemyBase>(hit.GetActor());

	/*if(ishit)
		WARNINGLOG(TEXT("%s actor hit: %s"), *this->GetName(), *actor->GetName())*/

	return actor != nullptr;
}

void AEnemyBase::ChaseTarget()
{
	MoveToActor(Target);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemyBase::ActivateCombatBox(const uint8 index)
{
	CombatBoxes[index]->SetCombatBoxCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("ActiveCombatBox"));
}

void AEnemyBase::DeactivateCombatBox(const uint8 index)
{
	CombatBoxes[index]->SetCombatBoxCollisionEnabled(ECollisionEnabled::NoCollision);
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("InactiveCombatBox"));
}
