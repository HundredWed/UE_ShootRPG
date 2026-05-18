#pragma once

#include "CoreMinimal.h"
#include "UE_RPG/UtilityMecro.h"
#include "Engine/StreamableManager.h"
#include "Item/ItemData.h"
#include "Item/Weapon/EquipmentData.h"
#include "Subsystems/WorldSubsystem.h"
#include "CPP_AkashicSubsystem.generated.h"

class ACPP_EquipmentBase;
class APickUpItem;

DECLARE_DELEGATE_OneParam(FOnWeaponSpawnedCallback, ACPP_EquipmentBase*);

UCLASS()
class UE_RPG_API UCPP_AkashicSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SpawnItemAsync(FName itemID, FVector spawnLocation);
	bool SpawnWeaponAsync(FName itemID, FVector spawnLocation, FOnWeaponSpawnedCallback onSpawnCompleted);

	FItemInfoTable* RequestItemInfo(const FName& itemId);
	FEquipmentInfoTable* RequestWeaponInfo(const FName& itemId);

private:

	void OnItemAssetsLoaded(FName itemID, FVector spawnLocation);
	void OnWeaponAssetsLoaded(FName itemID, FVector spawnLocation, FOnWeaponSpawnedCallback onSpawnCompleted);

public:
	
	UPROPERTY()
	UDataTable* ItemDataTable;

	UPROPERTY()
	UDataTable* WeaponDataTable;

	UPROPERTY()
	TSubclassOf<APickUpItem> EquipmentClass;
	UPROPERTY()
	TSubclassOf<APickUpItem> ItemClass;

	TArray<TSharedPtr<FStreamableHandle>> ActiveLoadHandles;
	TSubclassOf<APickUpItem> CurrentSpawnClass = nullptr;
};
