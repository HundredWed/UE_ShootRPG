#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentData.generated.h"

class UParticleSystem;
class USoundCue;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FEquipmentInfoTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName EquipmentID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 ATK;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<USkeletalMesh> ItemSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<UParticleSystem> FireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<UParticleSystem> BeamParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<USoundCue> EquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<USoundCue> AttackSound;
};