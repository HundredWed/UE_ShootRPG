// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CPP_InteractInterface.h"
#include "UE_RPG/UtilityMecro.h"
#include "CharacterStates.h"
#include "NonPlayerCharacterBase.generated.h"

class UHealthBarComponent;
class UMover;
class UAnimMontage;
class ACPP_Character;
class ACPP_NPCcontroller;
class UCameraComponent;
class UCPP_NPCAnimInstance;


UCLASS()
class UE_RPG_API ANonPlayerCharacterBase : public ACharacter, public ICPP_InteractInterface
{
	GENERATED_BODY()

public:
	
	ANonPlayerCharacterBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	/**component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
	UHealthBarComponent* HealthBarComponent;
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
	FORCEINLINE ECharacterTypes GetCharacterTypes() { return CharaterType; }
	virtual	void UpdateState() {};
	virtual void RequestInteract(AActor* interactor) override;

private:

	//npc전용
	void InitQuestSystem();

protected:
	
	/**states*/
	UPROPERTY(VisibleAnywhere, Category = "NPC Info")
	int32 MaxHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "NPC Info")
	int32 MaxMana = 100;
	UPROPERTY(VisibleAnywhere, Category = "NPC Info")
	int32 MaxStamina = 100;
	UPROPERTY(VisibleAnywhere, Category = "NPC Info")
	int32 ATK = 15;
	UPROPERTY(VisibleAnywhere, Category = "NPC Info")
	int32 DEF = 10;
	UPROPERTY(EditAnywhere, Category = "NPC Info")
	ECharacterTypes CharaterType = ECharacterTypes::Type_None;
	UPROPERTY(EditDefaultsOnly, Category = "NPC Info")
	float RespawnDelay = 6.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC Info")
	ENPCActionState ENPCActionState = ENPCActionState::Normal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC Info")
	ENPCState NPCState = ENPCState::Patrol;

	UPROPERTY(VisibleAnywhere, Category = "NPC Info | Target Info")
	ACPP_Character* Target = nullptr;

	UPROPERTY(EditAnyWhere, Category = "NPC Info")
	FName NPCID;

	UPROPERTY()
	ACPP_NPCcontroller* NPCController;

	UPROPERTY()
	FTransform CachedCameraTransform;

	FVector HitDir = FVector::Zero();
	FVector SpawnPos = FVector::Zero();
	float CurrentHP = 0;
	float DelfaultSpeed = 0.f;
	float SidStepSpeed = 0.f;
	float SidStepDis = 0.f;

	float CurrentTurningValue = 0.f;
	float TurnSpeed = 1.5f;
	float TurningValue = 0.f;
	float OverTurnValue = 35.f;

	FTimerHandle TimerHandle;
	FTimerHandle TurningHandle;

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void UpdateHealthPercent(float currentAmount);
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
	void SetHPMAX();
	void StopMove();
	void SetControlOwner(ANonPlayerCharacterBase* owner);
	
	UFUNCTION()
	void MoveDown();
	UFUNCTION()
	void MoveUp();

	
private:

	
};
