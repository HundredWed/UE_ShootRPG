#include "Item/Weapon/CPP_WeaponManager.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "Systems/CPP_AkashicSubsystem.h"
#include "Systems/CPP_SaveDataSubsystem.h"
#include "CPP_Character.h"

UCPP_WeaponManager::UCPP_WeaponManager()
{

}

void UCPP_WeaponManager::BeginPlay()
{
	Super::BeginPlay();
}

bool UCPP_WeaponManager::EquipWeapon(const FName& weaponId)
{
	if (weaponId.IsNone())
	{
		return false;
	}

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
	if (ACPP_Character* player = Cast<ACPP_Character>(GetOwner()))
	{
		weapon->SetOwner(GetOwner());
		weapon->StoreDamageUI(DamageUIActorClass);
		CurrentWeapon = weapon;

		weapon->Equip(player->GetMesh(), "weapon_socket_back");
		player->ApplyWeaponStat();
		WeaponStorage.Add(weapon->ItemInfoID, weapon);		
	}	
}

ACPP_WeaponBase* UCPP_WeaponManager::SpawnWeapon(TSubclassOf<ACPP_WeaponBase> weapon)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
		return nullptr;

	return  world->SpawnActor<ACPP_WeaponBase>(weapon);
}

float UCPP_WeaponManager::GetManaRegen()
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->ManaRegen;
	}
	return 0.0f;
}

float UCPP_WeaponManager::GetManaCost()
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->ManaCost;
	}
	return 0.0f;
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

void UCPP_WeaponManager::HoldWeapon(USceneComponent* Inparent, const FName& SocketName)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Equip(Inparent, "weapon_socket_r");
	}
}

void UCPP_WeaponManager::UnHoldWeapon(USceneComponent* Inparent, const FName& SocketName)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Equip(Inparent, "weapon_socket_back");
	}
}

float UCPP_WeaponManager::TriggerWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->Attack();
	}
	return -1.f;
}

void UCPP_WeaponManager::SetHiddenWeapon(bool newHidden)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->SetActorHiddenInGame(newHidden);
	}	
}

