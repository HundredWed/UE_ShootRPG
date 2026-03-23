#include "Item/Weapon/CPP_WeaponManager.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "GameFramework/Character.h"
#include "Systems/CPP_AkashicSubsystem.h"

UCPP_WeaponManager::UCPP_WeaponManager()
{

}

void UCPP_WeaponManager::BeginPlay()
{
	Super::BeginPlay();
}

bool UCPP_WeaponManager::EquipWeapon(const FName& weaponId)
{
	
	if (WeaponStorage.Contains(weaponId))
	{
		PrevWeapon = CurrentWeapon;

		PrevWeapon->SetActorHiddenInGame(true);

		ACPP_WeaponBase* weapon = *(WeaponStorage.Find(weaponId));
		weapon->SetActorHiddenInGame(false);
		CurrentWeapon = weapon;

		return true;
	}
	else
	{
		UWorld* world = GetWorld();
		if (!IsValid(world))
		{
			return false;
		}

		UCPP_AkashicSubsystem* AS = world->GetSubsystem<UCPP_AkashicSubsystem>();
		bool IsValidWeaponId = AS->SpawnWeaponAsync(weaponId, GetOwner()->GetActorLocation(), FOnWeaponSpawnedCallback::CreateUObject(this, &UCPP_WeaponManager::OnWeaponReady));
	
		if (IsValidWeaponId && !bClearWeaponTick)
		{
			bClearWeaponTick = true;
			world->GetTimerManager().SetTimer(ManagerTimer, this, &UCPP_WeaponManager::ClearWeaponGarbage, ClearWeaponTick);
		}

		return IsValidWeaponId;
	}
}

void UCPP_WeaponManager::TakeOffWeapon()
{
	CurrentWeapon->SetActorHiddenInGame(true);
}

void UCPP_WeaponManager::OnWeaponReady(ACPP_WeaponBase* weapon)
{
	weapon->SetOwner(GetOwner());
	ACharacter* player = Cast<ACharacter>(GetOwner());
	weapon->Equip(player->GetMesh(), "weapon_socket_back");
	WeaponStorage.Add(weapon->ItemInfoID, weapon);
	CurrentWeapon = weapon;
}

ACPP_WeaponBase* UCPP_WeaponManager::SpawnWeapon(TSubclassOf<ACPP_WeaponBase> weapon)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
		return nullptr;

	return  world->SpawnActor<ACPP_WeaponBase>(weapon);
}

void UCPP_WeaponManager::ClearWeaponGarbage()
{
	for (auto weapon : WeaponStorage)
	{
		if (weapon.Value)
		{
			if (weapon.Key == CurrentWeapon->ItemInfoID
				|| weapon.Key == PrevWeapon->ItemInfoID)
			{
				continue;
			}
			
			weapon.Value->Destroy();
			WeaponStorage.Remove(weapon.Key);
		}
	}

	bClearWeaponTick = false;
}


