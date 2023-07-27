// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/CPP_AnimInstance.h"
#include "CPP_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCPP_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<ACPP_Character>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}
	
}

void UCPP_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MyCharacterMovement)
	{
		Velocity = MyCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
		Angle = GetAngle2(); 
		isFalling = MyCharacterMovement->IsFalling();
		CharacterState = MyCharacter->GetCharacterState();
		Pich = MyCharacter->GetBaseAimRotation().Pitch;

		TurnInplace();
	}
	
}

void UCPP_AnimInstance::TurnInplace()
{
	if (GroundSpeed > 0 || CharacterState == ECharacterStateTypes::UnEquiped)
	{
		SetCurrentRotate();
	}
	else
	{
		
		MovementPrevYawOffset = MovementYawOffset;
		MovementYawOffset = MyCharacter->GetActorRotation().Yaw;
		const float subValueYaw = MovementYawOffset - MovementPrevYawOffset;

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - subValueYaw);

		//by GetBaseAimRotation
		Turning();

	}
}

void UCPP_AnimInstance::Turning()
{
	const float Turning{ GetCurveValue(TEXT("Turning")) };
	if (Turning > 0)
	{
		RotationCurvePrevFrame = RotationCurve;
		RotationCurve = GetCurveValue(TEXT("Rotation"));
		const float DeltaRotation{ RotationCurve - RotationCurvePrevFrame };

		// RootYawOffset > 0, -> Turning Left. RootYawOffset < 0, -> Turning Right.
		RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

		const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
		if (ABSRootYawOffset > 90.f)
		{
			const float YawExcess{ ABSRootYawOffset - 90.f };
			RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
		}
	}
}

double UCPP_AnimInstance::GetAngle()
{
	//In the first cycle, it is nullptr.
	//From the second cycle onwards, check the controller...???????????????????
	AController* Controller = TryGetPawnOwner()->GetController();
	if (IsValid(Controller) == false)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Controller is null!!"));
		return 0;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Controller is found!!"));
	}
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0); 

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	if (Velocity.Length() == 0)
	{
		return 0;
	}
	const FVector ActorDirection = Velocity.GetSafeNormal();

	const double CosTheta = FVector::DotProduct(ForwardDirection, ActorDirection);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(ForwardDirection, ActorDirection);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

double UCPP_AnimInstance::GetAngle2()
{
	FRotator AimRotation = MyCharacter->GetBaseAimRotation();
	FRotator MovementRotation =	UKismetMathLibrary::MakeRotFromX(Velocity);

	return UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,	AimRotation).Yaw; 
}

void UCPP_AnimInstance::SetCurrentRotate()
{
	/*FRotator newRotate = FRotator(MyCharacter->GetActorRotation().Pitch, 
		MyCharacter->GetActorRotation().Yaw + RootYawOffset, 
		MyCharacter->GetActorRotation().Roll); 
	MyCharacter->SetActorRotation(newRotate);*/ 

	RootYawOffset = 0;
	MovementYawOffset = MyCharacter->GetActorRotation().Yaw;
	MovementPrevYawOffset = MovementYawOffset;
	RotationCurvePrevFrame = 0.f;
	RotationCurve = 0.f;
}






