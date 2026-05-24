// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CPP_EquipmentManager.generated.h"


class ACPP_EquipmentBase;
class ACPP_DamageActor;

struct FEquipmentStat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_EquipmentManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_EquipmentManager();
	virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const FEquipmentStat* EquipEquipment(const FName& weaponid);
	void TakeOffWeapon();
	void OnWeaponReady(ACPP_EquipmentBase* weapon);
	ACPP_EquipmentBase* SpawnWeapon(TSubclassOf<ACPP_EquipmentBase> weapon);
	ACPP_EquipmentBase* GetCurrentWeapon() { return CurrentWeapon; }
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
	TMap<FName, ACPP_EquipmentBase*> WeaponStorage;

	/**데미지 ui*/
	UPROPERTY(EditAnywhere, Category = "WeaponInfo", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACPP_DamageActor> DamageUIActorClass;

	UPROPERTY()
	ACPP_EquipmentBase* PrevWeapon = nullptr;
	UPROPERTY()
	ACPP_EquipmentBase* CurrentWeapon = nullptr;

	FTimerHandle ManagerTimer;

	bool bClearWeaponTick = false;
};
