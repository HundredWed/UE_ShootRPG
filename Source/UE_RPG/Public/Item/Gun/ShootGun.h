#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon.h"
#include "ShootGun.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API AShootGun : public AWeapon
{
	GENERATED_BODY()

public:
	AShootGun();

	UPROPERTY(EditAnywhere)
		float MaxDir = 500.f;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, Category = "FireEffect")
		class USoundBase* ShootSound;
	UPROPERTY(EditAnywhere, Category = "FireEffect")
		class UParticleSystem* FireParticle;
	UPROPERTY(EditAnywhere, Category = "FireEffect")
		class UParticleSystem* BeamParticle;

	UPROPERTY(EditAnywhere, Category = "FireEffect")
		FVector ParticleSize;



	void PullTrigger();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	bool GunTrace(FHitResult& hitresult);
	AController* GetOwnerController();

private:
	
};
