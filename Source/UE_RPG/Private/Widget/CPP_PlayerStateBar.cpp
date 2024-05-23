// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_PlayerStateBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#define MAX 1

void UCPP_PlayerStateBar::InitStateBar(const int32 level, const float healt, const float maxHP, const float mana, const float stamina)
{
	UpdateLevel(level);
	UpdateHealthBarPercent(healt, maxHP);
	UpdateManaBarPercent(mana);

	if (IsValid(BackGroundBar))
	{
		BackGroundBar->SetPercent(MAX);
	}
}

void UCPP_PlayerStateBar::UpdateHealthBarPercent(const float currentHp , const float max)
{
	if (IsValid(PlayerHealth))
	{
		const float percent = currentHp / max;
		BGPercent = percent;
		PlayerHealth->SetPercent(percent);
		UpdatePercent = true;
	}

	UpdateHealthbarText(currentHp, max);

	UWorld* world = GetWorld();
	if (!IsValid(world) || (max == currentHp))
		return;

	FTimerHandle TimerHandle;
	world->GetTimerManager().SetTimer(TimerHandle, this, &UCPP_PlayerStateBar::UpdateBackGroundBar, 0.2f, false);
}

void UCPP_PlayerStateBar::UpdateManaBarPercent(const float percent)
{
	if (IsValid(PlayerMana))
	{
		PlayerHealth->SetPercent(percent);
	}
}

void UCPP_PlayerStateBar::UpdateLevel(int32 level)
{
	if (IsValid(PlayerLevel))
	{
		const FString String = FString::Printf(TEXT("%d"), level);
		const FText Text = FText::FromString(String);
		PlayerLevel->SetText(Text);
	}
}

void UCPP_PlayerStateBar::UpdateHealthbarText(const int32 current, const int32 max)
{
	if (IsValid(HealthAmount))
	{
		const FString String = FString::Printf(TEXT("%d / %d"), current, max);
		const FText Text = FText::FromString(String);
		HealthAmount->SetText(Text);
	}
}

void UCPP_PlayerStateBar::UpdateBackGroundBar()
{
	UpdatePercent = false;
	SetPercentTick();
}

void UCPP_PlayerStateBar::SetPercentTick()
{
	UWorld* world = GetWorld();
	if (!IsValid(world) || UpdatePercent || CheckPercent())
		return;

	const float precent = BackGroundBar->GetPercent() - SPEED2;
	BackGroundBar->SetPercent(precent);

	FTimerHandle TimerHandle;
	world->GetTimerManager().SetTimer(TimerHandle, this, &UCPP_PlayerStateBar::SetPercentTick, SPEED4, false);
}

bool UCPP_PlayerStateBar::CheckPercent()
{
	return BackGroundBar->GetPercent() < PlayerHealth->GetPercent() ? true : false;
}
