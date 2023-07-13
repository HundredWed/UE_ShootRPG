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
		float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float Angle;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
		ECharacterStateTypes CharacterState;


	double GetAngle();
};
