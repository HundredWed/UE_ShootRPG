// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC/HitEventInterface.h"
#include "UE_RPG/MyLogMecro.h"
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
		class UHealthBarComponent* HealthBarWidjet;

	/**montage*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* HitActionMontage_NoDamaged;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* HitActionMontage;

protected:
	
	virtual void BeginPlay() override;

	/**states*/
	UPROPERTY(EditAnywhere, Category = "NPC State")
		float Health = 0;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		float Mana = 0;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		float Stamina = 0;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		float ATK = 0;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		float DEF = 0;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		ECharacterTypes CharaterType = ECharacterTypes::Type_None;
	UPROPERTY(EditAnywhere, Category = "NPC State")
		ECharacterActionState CharaterActionState = ECharacterActionState::Normal;

	FVector HitDir = FVector::Zero();

public:	
	
	//virtual void Tick(float DeltaTime) override;

};
