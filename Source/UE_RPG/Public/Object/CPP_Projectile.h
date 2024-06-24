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

	UPROPERTY()
		class ACPP_AOE* AOE;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACPP_AOE> AOEclass;

protected:
	virtual void BeginPlay() override;
	

public:	
	//virtual void Tick(float DeltaTime) override;

	void ShootProjectile();

};
