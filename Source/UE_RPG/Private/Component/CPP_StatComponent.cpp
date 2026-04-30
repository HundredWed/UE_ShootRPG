#include "Component/CPP_StatComponent.h"
#include "Systems/CPP_SaveDataSubsystem.h"

UCPP_StatComponent::UCPP_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCPP_StatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//InitCharacterStats();
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

bool UCPP_StatComponent::IncreaseMP(const float value)
{
	OnOnUpdateMP.Broadcast(CharacterStats.CurrentMana, CharacterStats.MaxMana);
	return true;
}

bool UCPP_StatComponent::DecreaseMP(const float value)
{
	if (value <= 0.0f || CharacterStats.CurrentMana <= 0.0f)
	{
		return false;
	}

	CharacterStats.CurrentMana = FMath::Max(CharacterStats.CurrentMana - value, 0.0f);

	OnOnUpdateMP.Broadcast(CharacterStats.CurrentMana, CharacterStats.MaxMana);

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

