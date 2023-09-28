#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API ARifle : public AWeapon
{
	GENERATED_BODY()

public:
	ARifle();

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
	void ViewPointTrace(FHitResult& hitresult, FVector& endpoint);
	void GunTrace(FHitResult& hitresult, FVector& endpoint);
	
	void SpreadBulletRandomRange(FRotator& randDir);
	AController* GetOwnerController();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	/**between camera and player aiming issue Value*/
	float TraceStartPoint = 500.f;

	/**when hitresult is not, BeamParticle Direction*/
	FVector NoHitLocation = FVector::Zero();
};
