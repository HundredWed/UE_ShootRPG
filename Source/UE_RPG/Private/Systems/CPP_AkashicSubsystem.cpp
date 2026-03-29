#include "Systems/CPP_AkashicSubsystem.h"
#include "Systems/CPP_MyGameSettings.h"
#include "Engine/AssetManager.h"

#include "Item/PickUpItem.h"
//#include "Item/Weapon/CPP_WeaponBase.h"
#include "Item/Weapon/CPP_Rifle.h"//임시

void UCPP_AkashicSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UCPP_MyGameSettings* Settings = GetDefault<UCPP_MyGameSettings>();
	ItemDataTable = UCPP_MyGameSettings::LoadDataTableSafely(Settings->ItemData);
	WeaponDataTable = UCPP_MyGameSettings::LoadDataTableSafely(Settings->WeaponData);
}

void UCPP_AkashicSubsystem::Deinitialize()
{
    for (TSharedPtr<FStreamableHandle>& Handle : ActiveLoadHandles)
    {
        if (Handle.IsValid() && Handle->IsActive())
        {
            Handle->CancelHandle();
        }
    }

    ActiveLoadHandles.Empty();

    //파괴 되기전에 안전하게 로직를 수행하고 super 콜
    Super::Deinitialize();
}

void UCPP_AkashicSubsystem::SpawnItemAsync(FName itemID, FVector spawnLocation)
{
    FItemInfoTable* itemInfo = RequestItemInfo(itemID);
    if (itemInfo == nullptr)
    {
        return;
    }

    TArray<FSoftObjectPath> assetsToLoad;

    if (!itemInfo->ItemMesh.IsNull())
        assetsToLoad.AddUnique(itemInfo->ItemMesh.ToSoftObjectPath());


    if (assetsToLoad.Num() > 0)
    {
        FStreamableManager& streamable = UAssetManager::GetStreamableManager();

        TSharedPtr<FStreamableHandle> newHandle = streamable.RequestAsyncLoad(
            assetsToLoad,
            FStreamableDelegate::CreateUObject(this, &UCPP_AkashicSubsystem::OnItemAssetsLoaded, itemID, spawnLocation)
        );


        if (newHandle.IsValid())
        {
            ActiveLoadHandles.Add(newHandle);
        }
    }
    else
    {
        OnItemAssetsLoaded(itemID, spawnLocation);
    }
}

bool UCPP_AkashicSubsystem::SpawnWeaponAsync(FName itemID, FVector spawnLocation, FOnWeaponSpawnedCallback onSpawnCompleted)
{
    FItemInfoTable* itemInfo = RequestItemInfo(itemID);
    FEquipmentInfoTable* weaponInfo = RequestWeaponInfo(itemID);

    if (itemInfo == nullptr || weaponInfo == nullptr)
    {
        return false;
    }

    TArray<FSoftObjectPath> assetsToLoad;

    if (!weaponInfo->ItemSkeletalMesh.IsNull())
        assetsToLoad.AddUnique(weaponInfo->ItemSkeletalMesh.ToSoftObjectPath());
    if (!weaponInfo->FireParticle.IsNull())
        assetsToLoad.AddUnique(weaponInfo->FireParticle.ToSoftObjectPath());
    if (!weaponInfo->BeamParticle.IsNull())
        assetsToLoad.AddUnique(weaponInfo->BeamParticle.ToSoftObjectPath());
    if (!weaponInfo->EquipSound.IsNull())
        assetsToLoad.AddUnique(weaponInfo->EquipSound.ToSoftObjectPath());
    if (weaponInfo->AttackSound.IsNull())
        assetsToLoad.AddUnique(weaponInfo->AttackSound.ToSoftObjectPath());


    if (assetsToLoad.Num() > 0)
    {
        FStreamableManager& streamable = UAssetManager::GetStreamableManager();

        TSharedPtr<FStreamableHandle> newHandle = streamable.RequestAsyncLoad(
            assetsToLoad,
            FStreamableDelegate::CreateUObject(this, &UCPP_AkashicSubsystem::OnWeaponAssetsLoaded, itemID, spawnLocation, onSpawnCompleted)
        );

    
        if (newHandle.IsValid())
        {
            ActiveLoadHandles.Add(newHandle);
        }
    }
    else
    {
        OnWeaponAssetsLoaded(itemID, spawnLocation, onSpawnCompleted);
    }

    return true;
}

void UCPP_AkashicSubsystem::OnItemAssetsLoaded(FName itemID, FVector spawnLocation)
{
    APickUpItem* newItem = GetWorld()->SpawnActor<APickUpItem>(APickUpItem::StaticClass(), spawnLocation, FRotator::ZeroRotator);
    newItem->InitializePickUpItem();
}

void UCPP_AkashicSubsystem::OnWeaponAssetsLoaded(FName itemID, FVector spawnLocation, FOnWeaponSpawnedCallback onSpawnCompleted)
{
    ACPP_WeaponBase* newWeapon = GetWorld()->SpawnActor<ACPP_WeaponBase>(ACPP_Rifle::StaticClass(), spawnLocation, FRotator::ZeroRotator);
    newWeapon->InitWeaponInfo(itemID);

    onSpawnCompleted.ExecuteIfBound(newWeapon);
}

FItemInfoTable* UCPP_AkashicSubsystem::RequestItemInfo(const FName& itemId)
{
    if (ItemDataTable == nullptr)
    {
        return nullptr;
    }

	if (FItemInfoTable* data = ItemDataTable->FindRow<FItemInfoTable>(itemId, ""))
	{
		return data;
	}
	else
	{
		WARNINGLOG(TEXT("FItemInfoTable의 ID가 유효하지 않습니다!"));
		return nullptr;
	}
}

FEquipmentInfoTable* UCPP_AkashicSubsystem::RequestWeaponInfo(const FName& itemId)
{
    if (WeaponDataTable == nullptr)
    {
        return nullptr;
    }

    if (FEquipmentInfoTable* data = WeaponDataTable->FindRow<FEquipmentInfoTable>(itemId, ""))
    {
        return data;
    }
    else
    {
        WARNINGLOG(TEXT("FEquipmentInfoTable의 ID가 유효하지 않습니다!"));
        return nullptr;
    }
}


