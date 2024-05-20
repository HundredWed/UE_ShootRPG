// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/CPP_DamageComponent.h"
#include "Widget/NPC/CPP_DamageUI.h"

void UCPP_DamageComponent::UpdateDamageUI(const int32 amount)
{
	if (!IsValid(DamageUI))
	{
		DamageUI = Cast<UCPP_DamageUI>(GetUserWidgetObject());
	}

	if (IsValid(DamageUI))
	{
		DamageUI->UpdateWidget(amount);
	}
}
