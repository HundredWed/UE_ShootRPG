#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_CombatFeedbackComponent.generated.h"

class ACPP_DamageActor;

struct FDamageReceipt;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_CombatFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCPP_CombatFeedbackComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SpawnDamageUI(const FDamageReceipt& receipt);

private:

	void StoreDamageUI();	
	class ACPP_DamageActor* GetDamageActor();


private:

	/**데미지 ui*/
	UPROPERTY(EditAnywhere, Category = "Config", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACPP_DamageActor> DamageUIActorClass;
	
	UPROPERTY()
	TArray<ACPP_DamageActor*> DamageUIActors;

	int32 CurrentIndex = 0;
	
};
