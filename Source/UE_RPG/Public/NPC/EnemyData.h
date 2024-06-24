#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStates.h"
#include "EnemyData.generated.h"


USTRUCT(BlueprintType)
struct FEnemyInfoTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEnemyInfoTable() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		EEnemyCombatTypes CombatTypes = EEnemyCombatTypes::Dummy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		TArray<FName> SocketNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		TArray<FName> PlaySection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		float NoDamagedDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		float CombatDis = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		float ValidSightDis = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		float DelfaultSpeed = 630.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		float SidStepSpeed = 170.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		float SidStepDis = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		int32 MaxMana = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		int32 MaxStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		int32 ATK = 15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		int32 DEF = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
		ECharacterTypes CharaterType = ECharacterTypes::Type_None;
};