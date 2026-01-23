#pragma once
#include "CoreMinimal.h"
#include "CharacterStates.generated.h"

UENUM(BlueprintType)
enum class ECharacterTypes : uint8
{
	Player,
	NPC_Monster,
	NPC_Person,

	Type_None
};

//Player
//====================================================================
UENUM(BlueprintType)
enum class ECharacterStateTypes : uint8
{
	Death,

	Normal,/**기본(무기x)*/
	Equipped,/**무기 장착(무기o)*/
	Aim,/**무기 조준(무기o)*/
	UnEquipped,/**무기 수납(무기o)*/
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Normal,
	Action,/*can working**/
	SuperAction,/*can't all action**/
};
//====================================================================


//NPC
//====================================================================
UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Normal,
	Patrol,
	Combat,
	Chase,
	SideStep,
	Death
};

UENUM(BlueprintType)
enum class ENPCActionState : uint8
{
	Normal,
	Action,
	SuperAction
};
//====================================================================


//Enemy Only
//====================================================================
UENUM(BlueprintType)
enum class EEnemyCombatTypes : uint8
{
	Dummy,
	One_Hand,
	Two_Hand
};

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

public:
	FCharacterStats() {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Level = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxMana = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentMana = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxStamina = 150.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentStamina = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PlayerATK = 0.f;

	void Initialize()
	{
		CurrentHealth = MaxHealth;
		CurrentMana = MaxMana;
		CurrentStamina = MaxStamina;
	}

	float GetHealthRatio() const
	{
		return (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
	}

	float IncreaseHP(const float value) {
		CurrentHealth += value;
		float overValue = CurrentHealth - MaxHealth;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

		return FMath::Clamp(overValue, 0.f, overValue);
	}

	float DecreaseHP(const float value) {
		CurrentHealth -= value;
		return CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	}
};