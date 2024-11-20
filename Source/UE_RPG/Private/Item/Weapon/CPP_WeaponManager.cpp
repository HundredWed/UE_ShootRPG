#include "Item/Weapon/CPP_WeaponManager.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "GameFramework/Character.h"

UCPP_WeaponManager::UCPP_WeaponManager()
{

}

void UCPP_WeaponManager::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCPP_WeaponManager::EquipWeapon(const FName& weaponid, TSubclassOf<ACPP_WeaponBase> weapon)
{
	if (!WeaponStorage.IsEmpty() && WeaponStorage[weaponid])
	{
		CurrentWeapon->SetActorHiddenInGame(false);
		WeaponStorage[weaponid]->SetActorHiddenInGame(true);
		CurrentWeapon = WeaponStorage[weaponid];
	}
	else
	{
		UWorld* world = GetWorld();
		if (!IsValid(world))
			return;

		ACPP_WeaponBase* equip = SpawnWeapon(weapon);
		ACharacter* player = Cast<ACharacter>(GetOwner());
		equip->Equip(player->GetMesh(), "weapon_socket_back");
		AddWeapon(weaponid, equip);
	}
}

void UCPP_WeaponManager::AddWeapon(const FName& weaponid, ACPP_WeaponBase* weapon)
{
	WeaponStorage.Add(weaponid, weapon);
	CurrentWeapon = weapon;
}

ACPP_WeaponBase* UCPP_WeaponManager::SpawnWeapon(TSubclassOf<ACPP_WeaponBase> weapon)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
		return nullptr;

	return  world->SpawnActor<ACPP_WeaponBase>(weapon);
}


