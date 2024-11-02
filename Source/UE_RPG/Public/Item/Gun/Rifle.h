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
		float MaxDis = 500.f;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SpawnPoint;

	void PullTrigger();
	void ViewPointTrace(FHitResult& hitresult, FVector& endpoint);
	void GunTrace(FHitResult& hitresult, FVector& endpoint);
	void ShootEffect(const FVector& shootpoint);
	void TakeHit(FHitResult& hitresult , const FVector& shootpoint);


	void SpreadBulletRandomRange(FRotator& randDir);
	AController* GetOwnerController();
protected:
	virtual void BeginPlay() override;

private:
	/**between camera and player aiming issue Value*/
	float TraceStartPoint = 500.f;

	/**when hitresult is not, BeamParticle Direction*/
	FVector NoHitLocation = FVector::Zero();
};
