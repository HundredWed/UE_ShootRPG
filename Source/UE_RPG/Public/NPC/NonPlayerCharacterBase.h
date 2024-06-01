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

protected:
	
	/**states*/
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 MaxMana = 100;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 MaxStamina = 100;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 ATK = 15;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 DEF = 10;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		ECharacterTypes CharaterType = ECharacterTypes::Type_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC State")
		ENPCActionState ENPCActionState = ENPCActionState::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC State")
		ENPCState NPCState = ENPCState::Patrol;

	UPROPERTY(VisibleAnywhere, Category = "NPC State | Target Info")
		class ACPP_Character* Target = nullptr;

	UPROPERTY()
		class AAIController* NPCController;

	FVector HitDir = FVector::Zero();
	float CurrentHP = 0;

	FTimerHandle TimerHandle;
	FTimerHandle TurningHandle;

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual	void UpdateState();

	void UpdateHealthPercent(float currentAmount);
	void DieNPC();
	void SetStateDeath();
	void MoveToActor(const AActor* actor, const int acceptanceRadius = 3.f);
	void MoveToLocation(const FVector& pos, const int acceptanceRadius = 3.f);
	float PlayNPCMontage(UAnimMontage* montageToPlay);
	float CheckDist();
	void LookAtTarget(const FVector& targetpos);
	void TurnRight();
	void TurnLeft();
	void ClearTargetInfo();
	void SetHealthBarWidget(bool bvisibility);
	void SetHPMAX();
	void StopMove();
	


	UFUNCTION()
		void MoveDown();

	
private:

	float CurrentTurningValue = 0.f;
	float TurnSpeed = 5.f;
	float TurningValue = 0.f;

};
