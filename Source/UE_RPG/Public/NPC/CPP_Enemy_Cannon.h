#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyBase.h"
#include "CPP_Enemy_Cannon.generated.h"

UCLASS()
class UE_RPG_API ACPP_Enemy_Cannon : public AEnemyBase
{
	GENERATED_BODY()
	
public:

	ACPP_Enemy_Cannon();

	virtual void BeginPlay() override;
	virtual void Combat() override;
	virtual void ShootProjectile(bool bAEO = false, int32 index = 0) override;

	void AttackFunc();

public:

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		TArray<TSubclassOf<class ACPP_Projectile>> PTClassse;

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere, Category = "Enemy Info")
		FVector ParticleSize;
};
