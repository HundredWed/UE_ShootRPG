// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CPP_WeaponManager.generated.h"


class ACPP_WeaponBase;
class ACPP_DamageActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_WeaponManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_WeaponManager();
	virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool EquipWeapon(const FName& weaponid);
	void TakeOffWeapon();
	void OnWeaponReady(ACPP_WeaponBase* weapon);
	ACPP_WeaponBase* SpawnWeapon(TSubclassOf<ACPP_WeaponBase> weapon);
	ACPP_WeaponBase* GetCurrentWeapon() { return CurrentWeapon; }
	float GetManaRegen();
	float GetManaCost();

	void ClearWeaponGarbage();

	void HoldWeapon(USceneComponent* Inparent, const FName& SocketName);
	void UnHoldWeapon(USceneComponent* Inparent, const FName& SocketName);
	float TriggerWeapon();

	void SetHiddenWeapon(bool newHidden);

private:

	UPROPERTY(EditAnywhere, Category = "EquipManagerInfo")
	float ClearWeaponTick = 10.f;


	UPROPERTY()
	TMap<FName, ACPP_WeaponBase*> WeaponStorage;

	/**데미지 ui*/
	UPROPERTY(EditAnywhere, Category = "WeaponInfo", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACPP_DamageActor> DamageUIActorClass;

	UPROPERTY()
	ACPP_WeaponBase* PrevWeapon = nullptr;
	UPROPERTY()
	ACPP_WeaponBase* CurrentWeapon = nullptr;

	FTimerHandle ManagerTimer;

	bool bClearWeaponTick = false;
};
