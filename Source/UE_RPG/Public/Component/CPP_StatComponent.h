#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStates.h"
#include "Interface/CPP_SavableInterface.h"
#include "Structs/ST_DamageFeedback.h"
#include "CPP_StatComponent.generated.h"


DECLARE_DELEGATE_OneParam(FOnOnUpdateCharacterStateDelegate, const FCharacterStats&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateHPDelegate, const float, const float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecoverMPDelegate, const float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateMPDelegate, const float, const float);
DECLARE_MULTICAST_DELEGATE(FOnOverHeatDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCoolDownDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateStaminaDelegate, const float, const float);

class UCPP_SaveDataSubsystem;

struct FEquipmentStat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_StatComponent : public UActorComponent, public ICPP_SavableInterface
{
	GENERATED_BODY()

public:	
	
	UCPP_StatComponent();

	FOnOnUpdateCharacterStateDelegate OnUpdateCharacterState;

	FOnUpdateHPDelegate OnUpdateHP;
	FOnRecoverMPDelegate OnRecoverMP;
	FOnUpdateMPDelegate OnUpdateMP;
	FOnOverHeatDelegate OnOverHeat;
	FOnCoolDownDelegate OnCoolDown;
	FOnUpdateStaminaDelegate OnUpdateStamina;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitCharacterStats(bool bFill);
	void InitCharacterStats(FCharacterStats stats);
	float CalculateFinalDamage(const float damage, EDamageType damageType);
	void CalculateApplyDamage(FDamageReceipt& damageInfo);
	void ExecuteDotDamage(const float amount, const float delay,const float duration);

	bool IncreaseHP(const float value);
	bool DecreaseHP(const float value);

	void IncreaseMP();
	bool DecreaseMP();

	bool IncreaseStamina(const float value);
	bool DecreaseDodge();

	bool UpdateEXP(const float value);

	virtual void GatherSaveData(UCPP_SaveDataSubsystem* saveSystem) override;
	virtual void ApplySaveData(UCPP_SaveDataSubsystem* saveSystem) override;

	bool AddEquipmentStats(const FEquipmentStat* stat);

private:

	void StartRecoverMP();
	void OverHeatRecoverMP();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	FCharacterStats CharacterStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DodgeValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DashValue;

	FTimerHandle ManaRegenTimerHandle;
	float MPCost = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float UpdateInterval = 0.1f;


	bool bOverHeatFlag;
};
