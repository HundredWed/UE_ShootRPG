// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/WeaponAbiliys/WeaponAbilityBase.h"
#include "Item/WeaponAbiliys/RedRifleAbility.h"

void UWeaponAbilityBase::WeaponAbility()
{

}

UWeaponAbilityBase* UWeaponAbilityBase::GetAbility(const FName& id)
{
	if (id == "weapon_0001")
		return NewObject<URedRifleAbility>();
	/*else if (id == "weapon_0002")
		return NewObject<UNormalRifleAbility>();*/
	return nullptr;
}
