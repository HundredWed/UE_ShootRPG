// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterSpringArm.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCharacterSpringArm : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	UCharacterSpringArm();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//spring arm
	UFUNCTION()
	void UpdateSpringArmOffset(float NewYoffset, float DeltaTime);

	UPROPERTY(EditAnywhere)
		float InterpSpeed;

	float NewValue;


};
