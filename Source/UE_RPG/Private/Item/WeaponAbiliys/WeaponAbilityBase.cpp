// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponAbiliys/WeaponAbilityBase.h"
#include "Item/WeaponAbiliys/RedRifleAbility.h"
#include "Item/ItemData.h"

void UWeaponAbilityBase::WeaponAbility()
{

}

UWeaponAbilityBase* UWeaponAbilityBase::GetAbility(const uint8 id)
{
	if (id == (uint8)EWeaponAbilityID::EWA_Red)
		return NewObject<URedRifleAbility>();
	/*else if (id == (uint8)EWeaponAbilityID::EWA_Normal))
		return NewObject<UNormalRifleAbility>();*/
	return nullptr;
}
