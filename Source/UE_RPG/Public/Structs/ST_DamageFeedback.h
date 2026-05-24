#pragma once
#include "CoreMinimal.h"
#include "ST_DamageFeedback.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Normal,
	Critical,
	WeakPoint,
	WeakPointCrit,
	Immune
};

USTRUCT(BlueprintType)
struct FDamageReceipt
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float Damage = 0.f;

	UPROPERTY()
	FVector DamagedPoint = FVector::ZeroVector;

	UPROPERTY()
	FVector DamageLocation = FVector::ZeroVector;

	UPROPERTY()
	EDamageType DamageType = EDamageType::Normal;
};
