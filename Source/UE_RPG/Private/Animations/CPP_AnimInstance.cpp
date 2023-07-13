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
		Angle = GetAngle(); 
		isFalling = MyCharacterMovement->IsFalling();
		CharacterState = MyCharacter->GetCharacterState();
	}
}

double UCPP_AnimInstance::GetAngle()
{
	//In the first cycle, it is nullptr.
	//From the second cycle onwards, check the controller...???????????????????
	AController* Controller = TryGetPawnOwner()->GetController();
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is null!!"));
		return 0;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is found!!"));
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
