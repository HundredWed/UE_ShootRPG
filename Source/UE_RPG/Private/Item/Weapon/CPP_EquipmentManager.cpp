#include "Item/Weapon/CPP_EquipmentManager.h"
#include "Item/Weapon/CPP_EquipmentBase.h"
#include "Systems/CPP_AkashicSubsystem.h"
#include "Systems/CPP_SaveDataSubsystem.h"
#include "CPP_Character.h"

UCPP_EquipmentManager::UCPP_EquipmentManager()
{

}

void UCPP_EquipmentManager::BeginPlay()
{
	Super::BeginPlay();
}

const FEquipmentStat* UCPP_EquipmentManager::EquipEquipment(const FName& weaponId)
{
	if (weaponId.IsNone())
	{
		return nullptr;
	}

	if (ACPP_EquipmentBase** foundWeaponPtr = WeaponStorage.Find(weaponId))
	{
		ACPP_EquipmentBase* weapon = *foundWeaponPtr;
		if (!IsValid(weapon))
			return nullptr;

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetActorHiddenInGame(true);
		}

		PrevWeapon = CurrentWeapon;
		weapon->SetActorHiddenInGame(false);
		CurrentWeapon = weapon;

		return &weapon->EquipmentStat;
	}
	else
	{
		UWorld* world = GetWorld();
		if (!IsValid(world))
		{
			return nullptr;
		}

		UCPP_AkashicSubsystem* AS = world->GetSubsystem<UCPP_AkashicSubsystem>();
		bool IsValidWeaponId = AS->SpawnWeaponAsync(weaponId, GetOwner()->GetActorLocation(), FOnWeaponSpawnedCallback::CreateUObject(this, &UCPP_EquipmentManager::OnWeaponReady));
	
		if (IsValidWeaponId && !bClearWeaponTick)
		{
			bClearWeaponTick = true;
			world->GetTimerManager().SetTimer(ManagerTimer, this, &UCPP_EquipmentManager::ClearWeaponGarbage, ClearWeaponTick);
		}

		if (const auto* weaponInfo = AS->RequestEquipmentInfo(weaponId))
		{
			return &weaponInfo->EquipmentStat;
		}

		return nullptr;
	}
}

void UCPP_EquipmentManager::TakeOffWeapon()
{
	CurrentWeapon->SetActorHiddenInGame(true);
}

void UCPP_EquipmentManager::OnWeaponReady(ACPP_EquipmentBase* weapon)
{
	if (ACPP_Character* player = Cast<ACPP_Character>(GetOwner()))
	{
		weapon->SetOwner(GetOwner());
		CurrentWeapon = weapon;

		weapon->Equip(player->GetMesh(), "weapon_socket_back");
		WeaponStorage.Add(weapon->EquipmentStat.EquipmentID, weapon);
	}	
}

ACPP_EquipmentBase* UCPP_EquipmentManager::SpawnWeapon(TSubclassOf<ACPP_EquipmentBase> weapon)
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
		return nullptr;

	return  world->SpawnActor<ACPP_EquipmentBase>(weapon);
}

float UCPP_EquipmentManager::GetManaRegen()
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->EquipmentStat.ManaRegen;
	}
	return 0.0f;
}

float UCPP_EquipmentManager::GetManaCost()
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->EquipmentStat.ManaCost;
	}
	return 0.0f;
}

void UCPP_EquipmentManager::ClearWeaponGarbage()
{
	for (auto weapon : WeaponStorage)
	{
		if (weapon.Value)
		{
			if (weapon.Key == CurrentWeapon->EquipmentStat.EquipmentID
				|| weapon.Key == PrevWeapon->EquipmentStat.EquipmentID)
			{
				continue;
			}
			
			weapon.Value->Destroy();
			WeaponStorage.Remove(weapon.Key);
		}
	}

	bClearWeaponTick = false;
}

void UCPP_EquipmentManager::HoldWeapon(USceneComponent* Inparent, const FName& SocketName)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Equip(Inparent, "weapon_socket_r");
	}
}

void UCPP_EquipmentManager::UnHoldWeapon(USceneComponent* Inparent, const FName& SocketName)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Equip(Inparent, "weapon_socket_back");
	}
}

float UCPP_EquipmentManager::TriggerWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		return CurrentWeapon->Attack();
	}
	return -1.f;
}

void UCPP_EquipmentManager::SetHiddenWeapon(bool newHidden)
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->SetActorHiddenInGame(newHidden);
	}	
}

