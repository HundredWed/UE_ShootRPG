// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/CPP_EquipmentBase.h"
#include "CPP_Rifle.generated.h"


class UParticleSystem;
class USoundCue;

UCLASS()
class UE_RPG_API ACPP_Rifle : public ACPP_EquipmentBase
{
	GENERATED_BODY()

public:
	ACPP_Rifle();

	virtual float Attack() override;
	virtual void InitEquipmentInfo(const FName& itemID) override;
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

	FVector ParticleSize = { 1.f, 1.f, 1.f };
	float TraceStartPoint = 500.f;
	FVector NoHitLocation = FVector::Zero();
	FVector FirePoint = FVector::Zero();

private:
	
};
