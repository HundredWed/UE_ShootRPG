// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_EnemyCombatBox.generated.h"

UCLASS()
class UE_RPG_API ACPP_EnemyCombatBox : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_EnemyCombatBox();
	
	UPROPERTY(EditAnywhere)
		class UBoxComponent* CombatBox;

	UPROPERTY(EditAnywhere)
		class USoundBase* HitSound;

	UPROPERTY()
		class ACPP_Character* Player;

	bool bKnockBack = false;

	void SetCombatBoxCollisionEnabled(ECollisionEnabled::Type newType);
	FORCEINLINE void SetDamage(const float damage) { Damage = damage; }
protected:
	
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

	AController* GetOwnerController();

	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void InitCount() { Hicount = 0; }

private:
	
	UPROPERTY(EditAnywhere, Category = "Enemy Info", meta = (AllowPrivateAccess = "true"))
		float KnockBackVelocity = 1500.f;

	float Damage = 0.f;
	int8 Hicount  = 0;


};
