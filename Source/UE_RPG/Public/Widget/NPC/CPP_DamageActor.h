#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_DamageActor.generated.h"

class UCPP_DamageComponent;

struct FDamageReceipt;

UCLASS()
class UE_RPG_API ACPP_DamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_DamageActor();

	UPROPERTY(VisibleAnywhere, Category = "DamageWidget")
	UCPP_DamageComponent* DamageComponent;
	
protected:
	
	virtual void BeginPlay() override;

public:	
	
	//virtual void Tick(float DeltaTime) override;
	void UpdateDamageActor(const FDamageReceipt& receipt);
	bool IsActivateUI();
};
