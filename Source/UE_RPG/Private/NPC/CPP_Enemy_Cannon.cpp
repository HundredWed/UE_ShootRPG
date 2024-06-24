#include "NPC/CPP_Enemy_Cannon.h"
#include "CPP_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	LookAtTarget(Target->GetActorLocation());
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Enemy_Cannon::AttackFunc, (DELAY5 * 2.f), false);
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
