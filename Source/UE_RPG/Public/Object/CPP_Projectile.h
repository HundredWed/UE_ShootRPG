#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_Projectile.generated.h"

UCLASS()
class UE_RPG_API ACPP_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_Projectile();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Projectile;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* PTComponent;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* ProjectileTrail;

	UPROPERTY(EditAnywhere, Category = "PT Info")
		class USoundCue* HitSound;

	UPROPERTY()
		class ACPP_AOE* AOE;

	UPROPERTY(EditAnywhere, Category = "PT Info")
		TSubclassOf<class ACPP_AOE> AOEclass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "PT Info", meta = (AllowPrivateAccess = "true"))
		TArray<class UParticleSystem*> HitParticles;

	UPROPERTY(EditAnywhere, Category = "PT Info", meta = (AllowPrivateAccess = "true"))
		FVector HitParticleSize;

	UPROPERTY(EditAnywhere, Category = "PT Info", meta = (AllowPrivateAccess = "true"))
		float DamageMult = 1.f;

	UPROPERTY(EditAnywhere, Category = "PT Info", meta = (AllowPrivateAccess = "true"))
		float ExplosionRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float KnockBackVelocity = 1000.f;

	float Damage = 0.f;
	FVector ExplosionLocation{0.f,0.f,0.f};
	
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void ShootProjectile(bool bAEO = false);

	void UpdateDamage(const float damage) { Damage = damage * DamageMult; }

	UFUNCTION()
		void Explosion();
};
