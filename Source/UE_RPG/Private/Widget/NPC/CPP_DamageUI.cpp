// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/CPP_DamageUI.h"
#include "Components/TextBlock.h"

void UCPP_DamageUI::UpdateWidget(const float amount, EDamageType type)
{
	if (!IsValid(DamageText))
		return;

	bActivate = true;

	int32 displayDamage = FMath::Max(1, FMath::FloorToInt(amount));
	FText text = FText::AsNumber(displayDamage);
	DamageText->SetText(text);

	NormalEvent();

	/*switch (type)
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
	default:
		break;
	}*/

}

void UCPP_DamageUI::EndAnim()
{
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
	
}

void UCPP_DamageUI::WeakPointEvent()
{
	
}

void UCPP_DamageUI::WeakPointCritEvent()
{
	
}

void UCPP_DamageUI::ImmuneEvent()
{
	
}
