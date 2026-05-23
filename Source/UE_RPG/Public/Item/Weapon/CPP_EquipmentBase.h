#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemData.h"
#include "Item/Weapon/EquipmentData.h"
#include "Engine/DataTable.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_EquipmentBase.generated.h"

class UWeaponAbilityBase;
class USoundCue;
class ACPP_DamageActor;

UCLASS()
class UE_RPG_API ACPP_EquipmentBase : public AActor

{
	GENERATED_BODY()
	
public:	
	ACPP_EquipmentBase();

	UPROPERTY()
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
	FEquipmentStat EquipmentStat;

public:
	//virtual void Tick(float DeltaTime) override;

	/**공격딜레이 리턴*/
	virtual float Attack() { return 0.f; };
	virtual void InitWeaponInfo(const FName& itemID) {};
	virtual void Equip(USceneComponent* Inparent, const FName& SocketName);

	FORCEINLINE void AddFinalDamage(const float amount) { FinalDamage += amount; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float FinalDamage = 0.f;
};
