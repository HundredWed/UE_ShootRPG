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
	FCharacterStats StatData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	EEnemyCombatTypes CombatTypes = EEnemyCombatTypes::Dummy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	TArray<FName> SocketNames;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	TArray<FName> PlaySection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float CombatDis = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float ValidSightDis = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float DefaultSpeed = 630.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float SidStepSpeed = 170.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float SidStepDis = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Data")
	float RespawnDelay = 6.f;
};