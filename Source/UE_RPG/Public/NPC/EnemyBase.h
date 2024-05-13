// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NonPlayerCharacterBase.h"
#include "NPC/HitEventInterface.h"
#include "EnemyBase.generated.h"

class ACPP_Character;

UCLASS()
class UE_RPG_API AEnemyBase : public ANonPlayerCharacterBase, public IHitEventInterface
{
	GENERATED_BODY()

public:

	AEnemyBase();

	virtual bool GetHit(const FVector& targetLocation = FVector::Zero()) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Spawn();
	void UnSpawn();
	void SetTarget(ACPP_Character* target);
	FORCEINLINE void IsOrderfromSpawnArea(bool order) { bOrderfromSpawnArea = order; }

	/**ai*/
	void NoDamaged(const FVector& targetLocation);
	void BehaviorMode(ENPCState enemyState);
	float CheckDist();
	bool CanUpdateState();

	/**action*/
	void Patrol();
	void ChaseTarget();
	void ThinkAction();

	UFUNCTION(BlueprintCallable)
		void SetActionStateNormal();
	

protected:

	UFUNCTION()
	virtual	void UpdateState() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float NoDamagedDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float CombatDis = 100.f;

	UPROPERTY()
		class AAIController* EnemyController;

	FVector SpawnPos;
	int8 PrevRandomTarget = -1;
	bool bOrderfromSpawnArea = false;
};
