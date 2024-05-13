// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/CPP_DamageUI.h"
#include "Components/TextBlock.h"

void UCPP_DamageUI::UpdateWidget(const int32 amount)
{
	FText textcount = FText::AsNumber(amount);
	DamageText->SetText(textcount);

	FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCPP_DamageUI::SetHide, 0.8f, false);
}
