// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NonPlayerCharacterBase.h"
#include "NPC/HitEventInterface.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API AEnemyBase : public ANonPlayerCharacterBase, public IHitEventInterface
{
	GENERATED_BODY()

public:

	AEnemyBase();

	virtual bool GetHit(const FVector& targetLocation = FVector::Zero()) override;

	virtual void BeginPlay() override;

protected:

	UFUNCTION(BlueprintCallable)
		void SetActionStateNormal();

	void NoDamaged(const FVector& targetLocation);

	void LooAtTarget(const FVector& targetLocation);
	void TurnAtHitDir(const double theta);
	void TurnAtHitDir_Ver1(const FRotator& enemyrot);

private:

	UPROPERTY(EditAnywhere, Category = "NPC State", meta = (AllowPrivateAccess = "true"))
		double NoDamagedDistance = 500.f;

	float TurnSpeed = 15.f;
	float CurrentTurningValue = 0.f;
};
