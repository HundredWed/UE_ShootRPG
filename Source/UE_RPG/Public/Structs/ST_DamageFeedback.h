#pragma once
#include "CoreMinimal.h"
#include "ST_DamageFeedback.generated.h"

UENUM()
enum class EDamageType : uint8
{
	Normal,
	Critical,
	WeakPoint,
	WeakPointCrit,
	Immune
};

USTRUCT()
struct FDamageReceipt
{
	GENERATED_BODY()

public:

	float Damage = 0.f;
	FVector DamagedPoint = FVector::ZeroVector;
	EDamageType DamageType = EDamageType::Normal;
};
