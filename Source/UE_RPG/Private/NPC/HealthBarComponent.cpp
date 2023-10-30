// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/HealthBarComponent.h"
#include "Widget/NPC/NPCHealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (!IsValid(HealthBarWidget))
	{
		HealthBarWidget = Cast<UNPCHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->IsValidHealthBar())
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}

bool UHealthBarComponent::IsValidHealthBarWidget()
{
	return IsValid(HealthBarWidget);
}
