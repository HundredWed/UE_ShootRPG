// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterStates.h"
#include "CPP_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;//== beginplay
	virtual void NativeUpdateAnimation(float DeltaTime) override;//==tick

	UPROPERTY(BlueprintReadOnly)
		class ACPP_Character* MyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		class UCharacterMovementComponent* MyCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float GroundSpeed = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float Angle = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float MovementYawOffset = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float MovementPrevYawOffset = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float RootYawOffset = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float Pich = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		FVector Velocity{0,0,0,};

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isAiming = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isCrouching = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
		ECharacterStateTypes CharacterState;

	void TurnInplace();
	void Turning();

	double GetAngle();
	double GetAngle2();

	void SetCurrentRotate();

private:

	float Right = 90.f;
	float Left = -90.f;
	float InterpSpeedTurn = 20.f;

	float RotationCurve = 0;
	float RotationCurvePrevFrame = 0;

};
