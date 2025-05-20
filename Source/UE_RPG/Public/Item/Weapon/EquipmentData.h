#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentData.generated.h"

class UParticleSystem;
class USoundCue;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FEquipmentAssetTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	UParticleSystem* BeamParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	USoundCue* EquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	USoundCue* AttackSound;
};