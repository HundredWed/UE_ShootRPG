// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CPP_NPCAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NPC/NonPlayerCharacterBase.h"

void UCPP_NPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NPC = Cast<ANonPlayerCharacterBase>(TryGetPawnOwner());
	if (NPC)
	{
		MyCharacterMovement = NPC->GetCharacterMovement();
	}
}

void UCPP_NPCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MyCharacterMovement)
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



