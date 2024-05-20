// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/CPP_DamageUI.h"
#include "Components/TextBlock.h"

void UCPP_DamageUI::UpdateWidget(const int32 amount)
{
	if (!IsValid(DamageText))
		return;

	FText textcount = FText::AsNumber(amount);
	DamageText->SetText(textcount);

	int32 randomDir = FMath::RandRange(-1, 0);

	if (randomDir == 0)
	{
		PlayAnimation(DamageAnimRight);
	}
	else
	{
		PlayAnimation(DamageAnimLeft);
	}
	
	
}
