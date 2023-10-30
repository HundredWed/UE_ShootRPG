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

protected:
	
	/**states*/
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 MaxMana = 100;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 MaxStamina = 100;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 ATK = 10;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		int32 DEF = 10;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		ECharacterTypes CharaterType = ECharacterTypes::Type_None;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		ECharacterActionState CharaterActionState = ECharacterActionState::Normal;

	FVector HitDir = FVector::Zero();
	float CurrentHP = 0;

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void UpdateHealthPercent(float currentAmount);
	void DieNPC();
	void SetStateDeath();

	UFUNCTION()
		void MoveDown();

public:	
	
	

private:

	

};
