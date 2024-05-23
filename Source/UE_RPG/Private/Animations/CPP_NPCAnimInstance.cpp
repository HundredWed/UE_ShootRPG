// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CPP_NPCAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "NPC/EnemyBase.h"

void UCPP_NPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NPC = Cast<ANonPlayerCharacterBase>(TryGetPawnOwner());
	TypEnemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (IsValid(NPC))
	{
		MyCharacterMovement = NPC->GetCharacterMovement();
	}

	if(!IsValid(TypEnemy))
		WARNINGLOG(TEXT("is not valid TypEnemy in CPP_NPCAnimInstance!!"))
}

void UCPP_NPCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (IsValid(MyCharacterMovement))
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
		Angle = GetAngle2();
	}
}

float UCPP_NPCAnimInstance::GetAngle2()
{
	FRotator AimRotation = NPC->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Velocity);

	return UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}

void UCPP_NPCAnimInstance::AnimNotify_ActivateCombatBox_0(UAnimNotify* Notify)
{
	TypEnemy->ActivateCombatBox(BoxIndex0);
}

void UCPP_NPCAnimInstance::AnimNotify_DeactivateCombatBox_0(UAnimNotify* Notify)
{
	TypEnemy->DeactivateCombatBox(BoxIndex0);
}

void UCPP_NPCAnimInstance::AnimNotify_ActivateCombatBox_1(UAnimNotify* Notify)
{
	TypEnemy->ActivateCombatBox(BoxIndex1);
}

void UCPP_NPCAnimInstance::AnimNotify_DeactivateCombatBox_1(UAnimNotify* Notify)
{
	TypEnemy->DeactivateCombatBox(BoxIndex1);
}



