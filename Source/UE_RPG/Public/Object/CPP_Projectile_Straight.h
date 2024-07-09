// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/CPP_Projectile.h"
#include "CPP_Projectile_Straight.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API ACPP_Projectile_Straight : public ACPP_Projectile
{
	GENERATED_BODY()

public:
	ACPP_Projectile_Straight();
	virtual void Tick(float DeltaTime) override;
	virtual void ShootProjectile(bool bAEO = false);

	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		class USphereComponent* DamageEvent;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "PT Info", meta = (AllowPrivateAccess = "true"))
		float Speed = 10.f;

	int32 Hitcount = 0;
};
