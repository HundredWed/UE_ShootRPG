// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC/HitEventInterface.h"
#include "UE_RPG/UtilityMecro.h"
#include "CharacterStates.h"
#include "NonPlayerCharacterBase.generated.h"

UCLASS()
class UE_RPG_API ANonPlayerCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANonPlayerCharacterBase();

	/**component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
		class UHealthBarComponent* HealthBarComponent;
	UPROPERTY(EditAnyWhere)
		class UMover* Mover;

	/**montage*/
	UPROPERTY(VisibleAnywhere, Category = Montages)
		class UCPP_NPCAnimInstance* NPCAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* HitActionMontage_NoDamaged;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* HitActionMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* DeathActionMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* CombatActionMontage;

	bool bTurningLoop = false;

public:
	FORCEINLINE ENPCState GetNPCState() { return NPCState; }
	virtual	void UpdateState();
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
	UPROPERTY(VisibleAnywhere, Category = "NPC Info")
		ECharacterTypes CharaterType = ECharacterTypes::Type_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC Info")
		ENPCActionState ENPCActionState = ENPCActionState::Normal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NPC Info")
		ENPCState NPCState = ENPCState::Patrol;

	UPROPERTY(VisibleAnywhere, Category = "NPC Info | Target Info")
		class ACPP_Character* Target = nullptr;

	UPROPERTY()
		class ACPP_NPCcontroller* NPCController;

	FVector HitDir = FVector::Zero();
	float CurrentHP = 0;
	float DelfaultSpeed = 0.f;
	float SidStepSpeed = 0.f;
	float SidStepDis = 0.f;

	float CurrentTurningValue = 0.f;
	float TurnSpeed = 1.5f;
	float TurningValue = 0.f;

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
		void LookAtTarget(const FVector& targetpos);
	void TurnRight();
	void TurnLeft();
	void ClearTargetInfo();
	void SetHealthBarWidget(bool bvisibility);
	void SetHPMAX();
	void StopMove();
	void SetControlOwner(ANonPlayerCharacterBase* owner);
	
	UFUNCTION()
		void MoveDown();

private:


};
