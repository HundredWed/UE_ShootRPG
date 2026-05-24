// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

#include "CPP_Character.h"
#include "NPC/CPP_EnemyCombatBox.h"
#include "NPC/CPP_EnemySpawnArea.h"
#include "Animations/CPP_NPCAnimInstance.h"
#include "Object/Mover.h"
#include "Component/CPP_StatComponent.h"
#include "Interface/CPP_CombatReceiptReceiver.h"
#include "Components/SphereComponent.h"

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

	if (EnemyInfo.CombatTypes == EEnemyCombatTypes::Dummy || !IsValid(CombatBoxClass))
	{
		WARNINGLOG(TEXT("Please set CombatType!!"));
	}

	TArray<UActorComponent*> components = GetComponentsByTag(USphereComponent::StaticClass(), FName("WeakPoint"));
	for (UActorComponent* comp : components)
	{
		if (USphereComponent* sphere = Cast<USphereComponent>(comp))
		{
			WeakPointSpheres.Add(sphere);
		}
	}

	InitEnemyInfo();
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
		BehaviorMode(ENPCState::Death);
	}
	else if (!Target.IsValid())
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
			BehaviorMode(ENPCState::SideStep);
		}
		else if (dis > EnemyInfo.CombatDis)
		{
			BehaviorMode(ENPCState::Chase);
		}
		else
		{
			BehaviorMode(ENPCState::Combat);
		}
	}
}

void AEnemyBase::ExecuteHitEvent(const FDamageReceipt& receipt, AActor* damageCauser)
{
	SetHealthBarWidget(true);

	FDamageReceipt newReceipt = receipt;

	if (bDamageAble)
	{
		//약점부위 타격여부 판단
		for (USphereComponent* weakSphere : WeakPointSpheres)
		{
			if (weakSphere)
			{
				FVector sphereLocation = weakSphere->GetComponentLocation();
				const float sphereRadius = weakSphere->GetScaledSphereRadius();

				float dis = FVector::Dist(newReceipt.DamagedPoint, sphereLocation);
				if (dis <= sphereRadius)
				{
					switch (newReceipt.DamageType)
					{
					case EDamageType::Normal:
						newReceipt.DamageType = EDamageType::WeakPoint;
						break;
					case EDamageType::Critical:
						newReceipt.DamageType = EDamageType::WeakPointCrit;
						break;
					}
				}
			}
		}

		newReceipt.Damage = FinalDamage(receipt.Damage, newReceipt.DamageType);
		PlayNPCMontage(HitActionMontage);

		if (!Damaged(newReceipt.Damage))
		{
			DieNPC();
			BehaviorMode(ENPCState::Death);
		}
	}
	else
	{
		newReceipt.DamageType = EDamageType::Immune;
	}
	

	ICPP_CombatReceiptReceiver* CRR = Cast<ICPP_CombatReceiptReceiver>(damageCauser);
	CRR->SubmitReceipt(newReceipt);
}	

void AEnemyBase::ThinkAction()
{
	if (bRotatOnly)
	{
		if (!Target.IsValid())
			return;
		
		FRotator newRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
		SetActorRotation(newRot);
	}
	else
	{
		float dis = CheckDist();

		if (dis < NO_TARGET)
			return;

		if ((EnemyInfo.ValidSightDis > 0) && (dis < EnemyInfo.ValidSightDis) && IsCorwd())
		{
			SetActorTickEnabled(false);
			bCorwd = true;
			UpdateState();
		}

		if (dis < EnemyInfo.CombatDis)
		{
			SetActorTickEnabled(false);
			UpdateState();
		}
	}
}

void AEnemyBase::BehaviorMode(ENPCState enemyState)
{
	NPCState = enemyState;

	GetWorldTimerManager().ClearTimer(BehaviorTimerHandle);

	GetWorldTimerManager().SetTimer(BehaviorTimerHandle, this, &AEnemyBase::ExecuteBehavior, 0.05f, false);
}

void AEnemyBase::InitBehaviorState()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = EnemyInfo.DefaultSpeed;
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
	InitStats(EnemyInfo.StatData);
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

void AEnemyBase::ExecuteBehavior()
{
	InitBehaviorState();

	switch (NPCState)
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
		if (!bOrderfromSpawnArea)
			MySpawnArea->EnemyDeathCount(SpawnArrNum);
		break;
	default:
		break;
	}
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

	uint8 weapons = (uint8)EnemyInfo.CombatTypes;

	if (weapons != EnemyInfo.SocketNames.Num())
		return;

	for (uint8 i = 0; i < weapons; i++)
	{
		ACPP_EnemyCombatBox* weapon = World->SpawnActor<ACPP_EnemyCombatBox>(CombatBoxClass);
		weapon->SetDamage(EnemyInfo.StatData.CharacterATK);
		CombatBoxes.Push(weapon);
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		CombatBoxes[i]->AttachToComponent(GetMesh(), TransformRules, EnemyInfo.SocketNames[i]);
		CombatBoxes[i]->SetOwner(this);

		DISPLAYLOG(TEXT("WeaponReady!!"))
	}
}

void AEnemyBase::InitEnemyInfo()
{
	if (const UDataTable* enemyDataTable = EnemyHandle.DataTable)
	{
		const FEnemyInfoTable* info = enemyDataTable->FindRow<FEnemyInfoTable>(EnemyHandle.RowName, TEXT("유효하지 않은 ID입니다."));

		EnemyInfo = *info;
		InitStats(info->StatData);

		WeaponReady();
		SpawnPos = GetActorLocation();
		SetControlOwner(this);
		SetActorTickEnabled(false);
	}	
}

void AEnemyBase::Patrol()
{
	if (Target.IsValid())
		ClearTargetInfo();

	SetHealthBarWidget(false);
	bOrderfromSpawnArea = false;

	MoveToLocation(SpawnPos);
	InitStats(EnemyInfo.StatData);
}

void AEnemyBase::SideStep()
{
	GetCharacterMovement()->MaxWalkSpeed = EnemyInfo.SidStepSpeed;
	
	FVector rightVector = GetActorRightVector().GetSafeNormal();

	int32 randomDir = FMath::RandRange(-1, 0);
	float dir = randomDir == -1 ? -1.f : 1.f;
	NPCAnimInstance->Angle = dir;

	FVector sideVector = GetActorLocation() + (rightVector * dir * EnemyInfo.SidStepDis);

	MoveSide(sideVector);
}

void AEnemyBase::Combat()
{
	float animLength = 0.f;

	if (EnemyInfo.PlaySection.Num() > 0)
	{
		int32 random = FMath::RandRange(0, EnemyInfo.PlaySection.Num());
		if (random == EnemyInfo.PlaySection.Num())
		{
			BehaviorMode(ENPCState::SideStep);
			return;
		}
		else
			animLength = PlayNPCMontage(CombatActionMontage, EnemyInfo.PlaySection[random]);
	}
	else
	{
		animLength = PlayNPCMontage(CombatActionMontage);
	}

	if(Target.IsValid())
		LookAtTarget(Target->GetActorLocation());

	if (CanUpdateState() && animLength > 0)
	{
		// 임시
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
	MoveToActor(Target.Get());
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
	BehaviorMode(ENPCState::Normal);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::MoveUp, EnemyInfo.RespawnDelay, false);
}

void AEnemyBase::FinishMoveUpEvent()
{
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::ReSpawn, DELAY5, false);
}

void AEnemyBase::RequestInteract(AActor* interactor)
{
}
