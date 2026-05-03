#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EquipmentData.generated.h"

class UParticleSystem;
class USoundCue;
class UParticleSystem;
class UCPP_EquipmentAbilityDataAsset;

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None,
	Weapon,    
	OffensiveRing,   // 악세사리 1 (공격력 특화)
	DefensiveRing,	// 악세사리 2 (방어력 특화)

	EquipmentTypeCount //타입 갯수(=인벤토리 추가 슬롯 갯수)
};

USTRUCT(BlueprintType)
struct FEquipmentInfoTable : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName EquipmentID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EEquipmentType EquipmentType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 ATK;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 DEF;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float ManaCost;
	/**마나 응집력(사거리)*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float ManaDensity;
	/**마나 회복력*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float ManaRegen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<USkeletalMesh> ItemSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	FVector ParticleSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<UParticleSystem> FireParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<UParticleSystem> BeamParticle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<USoundCue> EquipSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<USoundCue> AttackSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TObjectPtr<UCPP_EquipmentAbilityDataAsset> EquipmentLogicAsset;
};