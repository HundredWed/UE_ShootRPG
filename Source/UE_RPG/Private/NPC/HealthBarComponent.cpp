// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/HealthBarComponent.h"
#include "Widget/NPC/NPCHealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UNPCHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->IsValideHealthBar())
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
