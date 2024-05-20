#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_DamageActor.generated.h"

UCLASS()
class UE_RPG_API ACPP_DamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_DamageActor();

	UPROPERTY(VisibleAnywhere, Category = "DamageWidget")
		class UCPP_DamageComponent* DamageComponent;

protected:
	
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;

public:	
	
	//virtual void Tick(float DeltaTime) override;
	void UpdateDamageActor(const FVector& pos, const int32 amount);
	void StartValidTime();

	UFUNCTION()
		void SetActorHidden();
};
