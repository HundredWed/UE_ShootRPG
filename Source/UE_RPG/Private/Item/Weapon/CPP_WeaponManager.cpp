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
	if (!IsValid(weapon))
	{
		WARNINGLOG(TEXT("is not valid weapon class in weaponmanager. please chack weapon subclass "))
		return;
	}
		

	if (!WeaponStorage.IsEmpty() && WeaponStorage[weaponid])
	{
		PrevWeapon = CurrentWeapon;

		CurrentWeapon->SetActorHiddenInGame(true);
		WeaponStorage[weaponid]->SetActorHiddenInGame(false);
		CurrentWeapon = WeaponStorage[weaponid];
	}
	else
	{
		ACPP_WeaponBase* equip = SpawnWeapon(weapon);
		equip->SetOwner(GetOwner());
		ACharacter* player = Cast<ACharacter>(GetOwner());
		equip->Equip(player->GetMesh(), "weapon_socket_back");
		AddWeapon(weaponid, equip);

		UWorld* world = GetWorld();
		if (!bClearWeaponTick || !IsValid(world))
		{
			bClearWeaponTick = true;
			world->GetTimerManager().SetTimer(ManagerTimer, this, &UCPP_WeaponManager::ClearWeaponGarbege, ClearWeaponTick);
		}
	}
}

void UCPP_WeaponManager::TakeOffWeapon()
{
	CurrentWeapon->SetActorHiddenInGame(true);
}

void UCPP_WeaponManager::AddWeapon(const FName& weaponid, ACPP_WeaponBase* weapon)
{
	WeaponStorage.Add(weaponid, weapon);
	WeaponIds.Add(weaponid);
	CurrentWeapon = weapon;
}

ACPP_WeaponBase* UCPP_WeaponManager::SpawnWeapon(TSubclassOf<ACPP_WeaponBase> weapon)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
		return nullptr;

	return  world->SpawnActor<ACPP_WeaponBase>(weapon);
}

void UCPP_WeaponManager::ClearWeaponGarbege()
{
	for (int32 i = 0; i < WeaponIds.Num(); i++)
	{
		if (WeaponStorage[WeaponIds[i]] == CurrentWeapon ||
			WeaponStorage[WeaponIds[i]] == PrevWeapon)
			continue;

		WeaponStorage[WeaponIds[i]]->Destroy();
		WeaponStorage.Remove(WeaponIds[i]);
	}

	bClearWeaponTick = false;
}


