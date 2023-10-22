// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/NPCHealthBar.h"
#include "Components/ProgressBar.h"

bool UNPCHealthBar::IsValideHealthBar()
{
	return IsValid(HealthBar);
}
