// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NonPlayerCharacterBase.h"
#include "NPC/HitEventInterface.h"
#include "NPC/EnemyData.h"
#include "EnemyBase.generated.h"

class ACPP_Character;

UCLASS()
class UE_RPG_API AEnemyBase : public ANonPlayerCharacterBase, public IHitEventInterface
{
	GENERATED_BODY()

public:

	AEnemyBase();

	bool bDirectly = false;

public:

	virtual bool GetHit(const FVector& targetLocation = FVector::Zero()) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Spawn(class ACPP_EnemySpawnArea* spawnarea);
	void UnSpawn();
	void SetTarget(ACPP_Character* target);
	void WeaponReady();
	void InitEnenmyInfo();
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
	virtual void Combat();
	bool IsCorwd();
	virtual void ShootProjectile(bool bAEO = true, int32 index = 0) {};
	void LookatTargetByTick();

	/**notify triggered*/
	void ActivateCombatBox(const uint8 index, bool knockBack = false);
	void DeactivateCombatBox(const uint8 index, bool knockBack = false);

	UFUNCTION(BlueprintCallable)
		void SetActionStateNormal();
	
	UFUNCTION()
		virtual	void UpdateState() override;

protected:

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ACPP_EnemyCombatBox> CombatBoxClass;

	UPROPERTY()
		TArray<class ACPP_EnemyCombatBox*> CombatBoxes;

	UPROPERTY()
		class ACPP_EnemySpawnArea* MySpawnArea;

	/**data table info*/
	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		UDataTable* EnemyDataTable;

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		FName EnemyID;/**same name*/

	UPROPERTY(VisibleAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		EEnemyCombatTypes CombatTypes = EEnemyCombatTypes::Dummy;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		TArray<FName> PlaySection;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		TArray<FName> SocketNames;

	float NoDamagedDistance = 0.f;
	float CombatDis = 0.f;
	float ValidSightDis = 0.f;

	FVector SpawnPos;
	bool bOrderfromSpawnArea = false;
	bool bCorwd = false;
	bool bRotatOnly = false;
	float CorwdTraceRadius = 0.f;

private:

	

	
};
