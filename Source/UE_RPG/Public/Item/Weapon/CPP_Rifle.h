// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "CPP_Rifle.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API ACPP_Rifle : public ACPP_WeaponBase
{
	GENERATED_BODY()

public:
	ACPP_Rifle();

	UPROPERTY(EditAnywhere, Category = "WeaponInfo|Effect")
		float MaxDis = 500.f;

	virtual void Attack() override;
	virtual void InitWeaponInfo() override;
	virtual void Equip(USceneComponent* Inparent, const FName& SocketName) override;

	void ViewPointTrace(FHitResult& hitresult, FVector& endpoint);
	void GunTrace(FHitResult& hitresult, FVector& endpoint);
	void ShootEffect(const FVector& shootpoint);
	void TakeHit(FHitResult& hitresult, const FVector& shootpoint);

	void SpreadBulletRandomRange(FRotator& randDir);
	AController* GetOwnerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo|Effect")
		FVector ParticleSize;
	UPROPERTY()
		class UParticleSystem* BeamParticle;
	UPROPERTY()
		class UParticleSystem* FireParticle;
	UPROPERTY()
		class USoundCue* ShootSound;
		UPROPERTY()
		class USoundCue* EquipSound;

private:
	/**between camera and player aiming issue Value*/
	float TraceStartPoint = 500.f;

	/**when hitresult is not, BeamParticle Direction*/
	FVector NoHitLocation = FVector::Zero();
	
	FVector FirePoint = FVector::Zero();
};
