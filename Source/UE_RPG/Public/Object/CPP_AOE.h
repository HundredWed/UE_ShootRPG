#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_AOE.generated.h"

UCLASS()
class UE_RPG_API ACPP_AOE : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_AOE();

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Ring;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Arae;


protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
	void SetAreaSpeed(const double time);

private:

	UPROPERTY(EditAnywhere, Category = "AOE Info" , meta = (AllowPrivateAccess = "true"))
		FVector AreaDefaultScale{ 0.0f,0.0f,0.01f };

	UPROPERTY(EditAnywhere, Category = "AOE Info", meta = (AllowPrivateAccess = "true"))
		FVector RingDefaultScale{ 10.f,10.f,10.f };

	double Speed = 0.f;
};
