// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CPP_WeaponManager.generated.h"


class ACPP_WeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_WeaponManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCPP_WeaponManager();
	virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipWeapon(const FName& weaponid,TSubclassOf<ACPP_WeaponBase> weapon);
	void AddWeapon(const FName& weaponid, ACPP_WeaponBase* weapon);
	ACPP_WeaponBase* SpawnWeapon(TSubclassOf<ACPP_WeaponBase> weapon);
	ACPP_WeaponBase* GetCurrntWeapon() { return CurrentWeapon; }

	

protected:

	UPROPERTY()
		TMap<FName, ACPP_WeaponBase*> WeaponStorage;

	UPROPERTY()
		ACPP_WeaponBase* PrevWeapon;
	UPROPERTY()
		ACPP_WeaponBase* CurrentWeapon = nullptr;
};
