// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/CPP_DamageUI.h"
#include "Components/TextBlock.h"

void UCPP_DamageUI::NativeConstruct()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_DamageUI::UpdateWidget(const float amount, EDamageType type)
{
	if (!IsValid(DamageText))
		return;

	SetVisibility(ESlateVisibility::Visible);
	bActivate = true;

	int32 displayDamage = FMath::Max(1, FMath::FloorToInt(amount));
	FText text = FText::AsNumber(displayDamage);
	DamageText->SetText(text);

	SetTextType(type);

	switch (type)
	{
	case EDamageType::Normal:
		NormalEvent();
		break;
	case EDamageType::Critical:
		CriticalEvent();
		break;
	case EDamageType::WeakPoint:
		WeakPointEvent();
		break;
	case EDamageType::WeakPointCrit:
		WeakPointCritEvent();
		break;
	case EDamageType::Immune:
		ImmuneEvent();
		break;
	/*default:
		NormalEvent();
		break;*/
	}

}

void UCPP_DamageUI::EndAnim()
{
	SetVisibility(ESlateVisibility::Hidden);
	bActivate = false;
}

void UCPP_DamageUI::NormalEvent()
{
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

void UCPP_DamageUI::CriticalEvent()
{
	PlayAnimation(DamageAnimCritical);
}

void UCPP_DamageUI::WeakPointEvent()
{
	NormalEvent();
}

void UCPP_DamageUI::WeakPointCritEvent()
{
	CriticalEvent();
}

void UCPP_DamageUI::ImmuneEvent()
{
	PlayAnimation(DamageAnimImmune);
}
