#include "Component/CPP_StatComponent.h"
#include "Systems/CPP_SaveDataSubsystem.h"

UCPP_StatComponent::UCPP_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCPP_StatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCPP_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPP_StatComponent::InitCharacterStats()
{
	if (OnUpdateCharacterState.IsBound())
	{
		OnUpdateCharacterState.Execute(CharacterStats);
	}
}

bool UCPP_StatComponent::IncreaseHP(const float value)
{
	if (value <= 0.0f)
	{
		return false;
	}

	bool bIsAlreadyFull = (CharacterStats.CurrentHealth >= CharacterStats.MaxHealth) ||
		FMath::IsNearlyEqual(CharacterStats.CurrentHealth, CharacterStats.MaxHealth);

	if (bIsAlreadyFull)
	{
		return false;
	}

	CharacterStats.CurrentHealth = FMath::Min(CharacterStats.CurrentHealth + value, CharacterStats.MaxHealth);

	OnUpdateHP.Broadcast(CharacterStats.CurrentHealth, CharacterStats.MaxHealth);

	return true;
}

bool UCPP_StatComponent::DecreaseHP(const float value)
{
	if (value <= 0.0f || CharacterStats.CurrentHealth <= 0.0f)
	{
		return false;
	}

	CharacterStats.CurrentHealth = FMath::Max(CharacterStats.CurrentHealth - value, 0.0f);

	OnUpdateHP.Broadcast(CharacterStats.CurrentHealth, CharacterStats.MaxHealth);

	return CharacterStats.CurrentHealth > 0.0f;
}

void UCPP_StatComponent::IncreaseMP()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(ManaRegenTimerHandle))
	{
		//UE_LOG(LogTemp, Display, TEXT("이미 회복 중"));
		return;
	}

	//UE_LOG(LogTemp, Display, TEXT("회복합니다"));

	if (bOverHeatFlag)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ManaRegenTimerHandle,
			this,
			&UCPP_StatComponent::OverHeatRecoverMP,
			UpdateInterval,
			true
		);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			ManaRegenTimerHandle,
			this,
			&UCPP_StatComponent::StartRecoverMP,
			UpdateInterval,
			true
		);
	}	
}

bool UCPP_StatComponent::DecreaseMP()
{
	if (MPCost <= 0.0f || FMath::IsNearlyZero(CharacterStats.CurrentMana))
	{
		return false;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(ManaRegenTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ManaRegenTimerHandle);
	}

	CharacterStats.CurrentMana = FMath::Max(CharacterStats.CurrentMana - MPCost, 0.0f);

	if (FMath::IsNearlyZero(CharacterStats.CurrentMana))
	{
		bOverHeatFlag = true;
		OnOverHeat.Broadcast();
	}

	OnUpdateMP.Broadcast(CharacterStats.CurrentMana, CharacterStats.MaxMana);

	return CharacterStats.CurrentMana > 0.0f;
}

bool UCPP_StatComponent::IncreaseStamina(const float value)
{
	OnUpdateStamina.Broadcast(CharacterStats.CurrentStamina, CharacterStats.MaxStamina);
	return true;
}

bool UCPP_StatComponent::DecreaseDodge()
{
	CharacterStats.CurrentStamina -= DodgeValue;
	OnUpdateStamina.Broadcast(CharacterStats.CurrentStamina, CharacterStats.MaxStamina);
	return true;
}

bool UCPP_StatComponent::UpdateEXP(const float value)
{
	CharacterStats.CurrentEXP += value;
	return true;
}

void UCPP_StatComponent::GatherSaveData(UCPP_SaveDataSubsystem* saveSystem)
{
	saveSystem->UpdateCharacterStatData(CharacterStats);
}

void UCPP_StatComponent::ApplySaveData(UCPP_SaveDataSubsystem* saveSystem)
{
	CharacterStats = saveSystem->GetCharacterStatData();
}

void UCPP_StatComponent::ApplyManaRegen(const float manaRegen)
{
	CharacterStats.ManaRegen += manaRegen;
}

void UCPP_StatComponent::ApplyManaCost(const float value)
{
	MPCost = value;
}

void UCPP_StatComponent::StartRecoverMP()
{
	float RecoverAmount = CharacterStats.ManaRegen * UpdateInterval;
	CharacterStats.CurrentMana += RecoverAmount;
	CharacterStats.CurrentMana = FMath::Clamp(CharacterStats.CurrentMana, 0.0f, CharacterStats.MaxMana);

	const float newPercent = CharacterStats.CurrentMana / CharacterStats.MaxMana;

	OnRecoverMP.Broadcast(newPercent);

	if (newPercent >= 1.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("회복 완료"));
		GetWorld()->GetTimerManager().ClearTimer(ManaRegenTimerHandle);
	}
}

void UCPP_StatComponent::OverHeatRecoverMP()
{
	const float overHeatRegen = 8.f;
	float RecoverAmount = overHeatRegen * UpdateInterval;
	CharacterStats.CurrentMana += RecoverAmount;
	CharacterStats.CurrentMana = FMath::Clamp(CharacterStats.CurrentMana, 0.0f, CharacterStats.MaxMana);

	const float newPercent = CharacterStats.CurrentMana / CharacterStats.MaxMana;

	OnRecoverMP.Broadcast(newPercent);

	if (newPercent >= 0.3f)
	{
		OnCoolDown.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(ManaRegenTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(
			ManaRegenTimerHandle,
			this,
			&UCPP_StatComponent::StartRecoverMP,
			UpdateInterval,
			true
		);
	}
}

