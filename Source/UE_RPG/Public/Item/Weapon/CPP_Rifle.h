// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "CPP_Rifle.generated.h"


class UParticleSystem;
class USoundCue;

UCLASS()
class UE_RPG_API ACPP_Rifle : public ACPP_WeaponBase
{
	GENERATED_BODY()

public:
	ACPP_Rifle();

	float MaxDis = 500.f;
	FVector ParticleSize = { 1.f, 1.f, 1.f };
	float FireRate = 0.5f;

	virtual float Attack() override;
	virtual void InitWeaponInfo(const FName& itemID) override;
	virtual void Equip(USceneComponent* Inparent, const FName& SocketName) override;

	void ViewPointTrace(FHitResult& hitresult, FVector& endpoint);
	void GunTrace(FHitResult& hitresult, FVector& endpoint);
	void ShootEffect(const FVector& shootpoint);
	void TakeHit(FHitResult& hitresult, const FVector& shootpoint);

	void SpreadBulletRandomRange(FRotator& randDir);
	AController* GetOwnerController();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY()
	UParticleSystem* BeamParticle;
	UPROPERTY()
	UParticleSystem* FireParticle;
	UPROPERTY()
	USoundCue* ShootSound;
	UPROPERTY()
	USoundCue* EquipSound;

private:
	/**between camera and player aiming issue Value*/
	float TraceStartPoint = 500.f;

	/**when hitresult is not, BeamParticle Direction*/
	FVector NoHitLocation = FVector::Zero();
	
	FVector FirePoint = FVector::Zero();
};
