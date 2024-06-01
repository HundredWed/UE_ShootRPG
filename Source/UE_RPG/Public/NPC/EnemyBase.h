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


public:

	virtual bool GetHit(const FVector& targetLocation = FVector::Zero()) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Spawn(class ACPP_EnemySpawnArea* spawnarea);
	void UnSpawn();
	void SetTarget(ACPP_Character* target);
	void WeaponReady();
	FORCEINLINE void IsOrderfromSpawnArea(bool order) { bOrderfromSpawnArea = order; }

	/**ai*/
	void NoDamaged(const FVector& targetLocation);
	void ThinkAction();
	void BehaviorMode(ENPCState enemyState);
	void InitBehaviorState();
	bool CanUpdateState();
	
	/**action*/
	void Patrol();
	void ChaseTarget();
	void SideStep();
	void Combat();
	void ApproachToTarget();
	bool IsCorwd();

	/**notify triggered*/
	void ActivateCombatBox(const uint8 index);
	void DeactivateCombatBox(const uint8 index);

	UFUNCTION(BlueprintCallable)
		void SetActionStateNormal();
	

protected:

	UFUNCTION()
	virtual	void UpdateState() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float NoDamagedDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float CombatDis = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float ValidSightDis = 500.f;

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ACPP_EnemyCombatBox> CombatBoxClass;

	UPROPERTY()
		TArray<class ACPP_EnemyCombatBox*> CombatBoxes;

	UPROPERTY()
		class ACPP_EnemySpawnArea* MySpawnArea;
	
	/**data table info*/
	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		EEnemyCombatTypes CombatTypes = EEnemyCombatTypes::Dummy;
	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		TArray<FName> SocketNames;


	FVector SpawnPos;
	int8 PrevRandomTarget = -1;
	bool bOrderfromSpawnArea = false;
	float CorwdTraceRadius = 0.f;
};
