// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterStates.h"
#include "CPP_NPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_NPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	//virtual void NativeUpdateAnimation(float DeltaTime) override;

	void SetNPCActionState(ECharacterActionState state);

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class AEnemyBase* NPCPawn;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ECharacterActionState NPCActionState = ECharacterActionState::Normal;
};
