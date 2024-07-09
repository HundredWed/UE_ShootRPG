#include "NPC/CPP_Enemy_Cannon.h"
#include "CPP_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

#include "Object/CPP_Projectile.h"

#define ATTACK1 0  
#define ATTACK2 1  
#define ATTACK3 2  

ACPP_Enemy_Cannon::ACPP_Enemy_Cannon()
{
}

void ACPP_Enemy_Cannon::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Enemy_Cannon::Combat()
{
	bool overTurn = Target->GetPlayerMoveState();
	LookAtTarget(Target->GetActorLocation(), overTurn);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Enemy_Cannon::AttackFunc, (DELAY5 * 2.f), false);
}

void ACPP_Enemy_Cannon::ShootProjectile(bool bAEO, int32 index)
{
	UWorld* world = GetWorld();

	bool socket = GetMesh()->DoesSocketExist("Muzzle_Front");

	if (socket && PTClassse.Num() > 0 && IsValid(Target))
	{
		FVector loc = GetMesh()->GetSocketLocation("Muzzle_Front");
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(loc, Target->GetActorLocation());

		if (IsValid(FireParticle))
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, loc, FRotator::ZeroRotator, ParticleSize);
		}

		if (bAEO)
		{
			rot = GetMesh()->GetSocketRotation("Muzzle_Front");

			const float dis = CheckDist();
			if (dis < CombatDis)
			{
				rot.Pitch += ((CombatDis - dis) / 40.f);
			}
		}

		ACPP_Projectile* pt = world->SpawnActor<ACPP_Projectile>(PTClassse[index], loc, rot);
		pt->ShootProjectile(bAEO);
		pt->SetOwner(this);
		pt->UpdateDamage(ATK);
	}
}

void ACPP_Enemy_Cannon::AttackFunc()
{
	if (!IsValid(Target) || PlaySection.Num() <= 0)
		return;
	float animLength = 0;
	float dis = CheckDist();

	const float middle = CombatDis * 0.70f;
	const float near = CombatDis * 0.15f;

	if (dis < CombatDis && dis > middle)
	{
		LookatTargetByTick();
		animLength = PlayNPCMontage(CombatActionMontage, PlaySection[ATTACK1]);
	}
	else if (dis < middle && dis > near)
	{
		ShootProjectile(false, ATTACK2);
		animLength = PlayNPCMontage(CombatActionMontage, PlaySection[ATTACK2]);
	}
	else if (dis < near)
	{
		animLength = PlayNPCMontage(CombatActionMontage, PlaySection[ATTACK3]);
	}
	else
	{
		animLength = DELAY1;
	}

	if (CanUpdateState() && animLength > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::UpdateState, animLength, false);
	}
}
