// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CPP_InteractInterface.h"
#include "NPC/HitEventInterface.h"
#include "UE_RPG/UtilityMecro.h"
#include "NonPlayerCharacterBase.generated.h"

class UHealthBarComponent;
class UMover;
class UAnimMontage;
class ACPP_Character;
class ACPP_NPCcontroller;
class UCameraComponent;
class UCPP_NPCAnimInstance;
class UCPP_StatComponent;



UCLASS()
class UE_RPG_API ANonPlayerCharacterBase : public ACharacter, public ICPP_InteractInterface, public IHitEventInterface
{
	GENERATED_BODY()

public:
	
	ANonPlayerCharacterBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	/**component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
	UHealthBarComponent* HealthBarComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
	UCPP_StatComponent* StatComponent;

	UPROPERTY(EditAnyWhere)
	UMover* Mover;

	/**montage*/
	UPROPERTY(VisibleAnywhere, Category = "NPC Info|Montage")
	UCPP_NPCAnimInstance* NPCAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category = "NPC Info|Montage")
	UAnimMontage* HitActionMontage_NoDamaged;
	UPROPERTY(EditDefaultsOnly, Category = "NPC Info|Montage")
	UAnimMontage* HitActionMontage;
	UPROPERTY(EditDefaultsOnly, Category = "NPC Info|Montage")
	UAnimMontage* DeathActionMontage;
	UPROPERTY(EditDefaultsOnly, Category = "NPC Info|Montage")
	UAnimMontage* CombatActionMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* DialogueCameraPreview;

	bool bTurningLoop = false;

public:
	FORCEINLINE ENPCState GetNPCState() { return NPCState; }
	virtual	void UpdateState() {};
	virtual void RequestInteract(AActor* interactor) override;
	virtual ECharacterTypes GetType() override;

private:

	//npc전용
	void InitQuestSystem();

protected:

	ECharacterTypes CharacterType;

	ENPCActionState ENPCActionState = ENPCActionState::Normal;
	ENPCState NPCState = ENPCState::Patrol;
	ENPCState PrevNPCState = ENPCState::Patrol;
	

	UPROPERTY(EditAnyWhere, Category = "NPC Info")
	FName NPCID;

	UPROPERTY()
	ACPP_NPCcontroller* NPCController;

	UPROPERTY()
	FTransform CachedCameraTransform;

	TWeakObjectPtr<ACPP_Character> Target;

	FVector HitDir = FVector::Zero();
	FVector SpawnPos = FVector::Zero();
	
	float CurrentTurningValue = 0.f;
	float TurnSpeed = 1.5f;
	float TurningValue = 0.f;
	float OverTurnValue = 35.f;

	FTimerHandle TimerHandle;
	FTimerHandle TurningHandle;

	virtual void BeginPlay() override;

	void InitStats(const FCharacterStats& stats);
	void RecoverHP(const float value);
	bool Damaged(const float value);
	float FinalDamage(const float value, EDamageType damageType);

	void DieNPC();
	void SetStateDeath();
	void MoveToActor(const AActor* actor, const int acceptanceRadius = 3.f);
	void MoveToLocation(const FVector& pos, const int acceptanceRadius = 3.f);
	void MoveSide(const FVector& pos);
	float PlayNPCMontage(UAnimMontage* montageToPlay, const FName& section = "null");
	float CheckDist();

	UFUNCTION()
	void LookAtTarget(const FVector& targetpos, bool boverTurn = false);

	void TurnRight();
	void TurnLeft();
	void ClearTargetInfo();
	void SetHealthBarWidget(bool bvisibility);
	void StopMove();
	void SetControlOwner(ANonPlayerCharacterBase* owner);
	
	UFUNCTION()
	void MoveDown();
	UFUNCTION()
	void MoveUp();

	
private:

	void UpdateCharacterStats(const FCharacterStats& stats);
	void UpdateHealthPercent(const float currentAmount, const float maxAmount);
};
