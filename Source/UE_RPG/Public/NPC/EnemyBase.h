// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NonPlayerCharacterBase.h"
#include "NPC/EnemyData.h"
#include "EnemyBase.generated.h"

class ACPP_Character;
class ACPP_EnemySpawnArea;
class ACPP_EnemyCombatBox;

UCLASS()
class UE_RPG_API AEnemyBase : public ANonPlayerCharacterBase
{
	GENERATED_BODY()

public:

	AEnemyBase();

	bool bDirectly = false;

public:

	virtual void ExecuteHitEvent(FDamageReceipt& receipt, AController* eventInstigator, AActor* damageCauser) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Spawn(class ACPP_EnemySpawnArea* spawnarea, const int32 arrNum);
	void ReSpawn();
	void SetTarget(ACPP_Character* target);
	void WeaponReady();
	void InitEnemyInfo();
	FORCEINLINE void IsOrderfromSpawnArea(bool order) { bOrderfromSpawnArea = order; }

	/**ai*/
	void ThinkAction();
	void BehaviorMode(ENPCState enemyState);
	void InitBehaviorState();
	bool CanUpdateState();
	void ExecuteBehavior();
	
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
	UFUNCTION()
		void FinishMoveDownEvent();
	UFUNCTION()
		void FinishMoveUpEvent();

	virtual void RequestInteract(AActor* interactor) override;
protected:

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACPP_EnemyCombatBox> CombatBoxClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle EnemyHandle;


	UPROPERTY()
	TArray<ACPP_EnemyCombatBox*> CombatBoxes;

	UPROPERTY()
	ACPP_EnemySpawnArea* MySpawnArea;
	
	UPROPERTY()
	FEnemyInfoTable EnemyInfo;

	int32 SpawnArrNum = 0;

	bool bOrderfromSpawnArea = false;
	bool bCorwd = false;
	bool bRotatOnly = false;
	float CorwdTraceRadius = 0.f;

	FTimerHandle BehaviorTimerHandle;
private:
};
