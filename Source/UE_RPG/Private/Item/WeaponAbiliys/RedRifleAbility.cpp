// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponAbiliys/RedRifleAbility.h"

void URedRifleAbility::WeaponAbility()
{
	GEngine->AddOnScreenDebugMessage(
		INDEX_NONE,
		30.f,
		FColor::Red,
		FString::Printf(TEXT("Fire!!!")));
}
