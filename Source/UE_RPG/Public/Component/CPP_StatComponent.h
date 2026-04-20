#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStates.h"
#include "CPP_StatComponent.generated.h"


DECLARE_DELEGATE_OneParam(FOnOnUpdateCharacterStateDelegate, const FCharacterStats&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateHPDelegate, const float, const float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateManaDelegate, const float, const float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateStaminaDelegate, const float, const float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCPP_StatComponent();

	FOnOnUpdateCharacterStateDelegate OnUpdateCharacterState;

	FOnUpdateHPDelegate OnUpdateHP;
	FOnUpdateManaDelegate OnOnUpdateMP;
	FOnUpdateStaminaDelegate OnUpdateStamina;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IncreaseHP(const float value);
	bool DecreaseHP(const float value);

	bool IncreaseMP(const float value);
	bool DecreaseMP(const float value);

	bool IncreaseStamina(const float value);
	bool DecreaseDodge();

	bool UpdateEXP(const float value);

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FCharacterStats CharacterStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DodgeValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DashValue;
};
