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
#include "Object/Mover.h"

#define NO_TARGET 0

AEnemyBase::AEnemyBase()
{
	CorwdTraceRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();

	Mover->FinishDown.BindUFunction(this, FName("FinishMoveDownEvent"));
	Mover->FinishUp.BindUFunction(this, FName("FinishMoveUpEvent"));
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
	if (NPCState == ENPCState::Death || NPCState == ENPCState::Normal)
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

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(NPCState == ENPCState::Death)
		BehaviorMode(NPCState = ENPCState::Death);

	return 0.0f;
}

void AEnemyBase::NoDamaged(const FVector& targetLocation)
{
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Ouch!!"));
	PlayNPCMontage(HitActionMontage_NoDamaged);
}

void AEnemyBase::ThinkAction()
{
	if (bRotatOnly)
	{
		if (!IsValid(Target))
			return;
		
		FRotator newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		SetActorRotation(newRot);
	}
	else
	{
		float dis = CheckDist();

		if (dis < NO_TARGET)
			return;

		if ((ValidSightDis > 0) && (dis < ValidSightDis) && IsCorwd())
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
}

void AEnemyBase::BehaviorMode(ENPCState enemyState)
{
	InitBehaviorState();

	switch (enemyState)
	{
	case ENPCState::Normal:
		break;
	case ENPCState::Patrol:
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
		if(!bOrderfromSpawnArea)
			MySpawnArea->EnemyDeathCount(SpawnArrNum);
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
	bRotatOnly = false;
	StopMove();
}

void AEnemyBase::Spawn(ACPP_EnemySpawnArea* spawnarea, const int32 arrNum)
{
	MySpawnArea = spawnarea;
	SpawnArrNum = arrNum;
	SetActorHiddenInGame(false);
}

void AEnemyBase::ReSpawn()
{
	CurrentHP = MaxHealth;
	UpdateHealthPercent(CurrentHP);
	SetHealthBarWidget(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Target = MySpawnArea->GetTarget();
	MySpawnArea->EnemySpawnCount(SpawnArrNum);
	UpdateState();
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
	{
		ClearTargetInfo();
		UpdateState();
		return;
	}

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
		CombatBoxes[i]->SetOwner(this);

		DISPLAYLOG(TEXT("WeaponReady!!"))
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
		SidStepSpeed = info->SidStepSpeed;
		DelfaultSpeed = info->DelfaultSpeed;
		SidStepDis = info->SidStepDis;

		MaxHealth = info->MaxHealth;
		CurrentHP = MaxHealth;
		MaxMana = info->MaxMana;
		ATK = info->ATK;
		DEF = info->DEF;
		CharaterType = info->CharaterType;

		WeaponReady();
		SpawnPos = GetActorLocation();
		SetControlOwner(this);
		SetActorTickEnabled(false);
	}

	
}

void AEnemyBase::Patrol()
{
	if (Target != nullptr)
		ClearTargetInfo();

	SetHealthBarWidget(false);
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

	FVector sideVector = GetActorLocation() + (rightVector * dir * SidStepDis);

	MoveSide(sideVector);
}

void AEnemyBase::Combat()
{
	float animLength = 0.f;

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

void AEnemyBase::LookatTargetByTick()
{
	bRotatOnly = true;
	SetActorTickEnabled(true);
}

void AEnemyBase::ChaseTarget()
{
	MoveToActor(Target);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemyBase::ActivateCombatBox(const uint8 index, bool knockBack)
{
	CombatBoxes[index]->bKnockBack = knockBack;
	CombatBoxes[index]->SetCombatBoxCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("ActiveCombatBox"));
}

void AEnemyBase::DeactivateCombatBox(const uint8 index, bool knockBack)
{
	CombatBoxes[index]->bKnockBack = knockBack;
	CombatBoxes[index]->SetCombatBoxCollisionEnabled(ECollisionEnabled::NoCollision);
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("InactiveCombatBox"));
}

void AEnemyBase::FinishMoveDownEvent()
{
	BehaviorMode(NPCState = ENPCState::Normal);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::MoveUp, RespawnDelay, false);
}

void AEnemyBase::FinishMoveUpEvent()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::ReSpawn, DELAY5, false);
}