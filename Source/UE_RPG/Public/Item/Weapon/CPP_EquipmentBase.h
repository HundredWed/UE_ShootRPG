#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Weapon/EquipmentData.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_EquipmentBase.generated.h"

class UWeaponAbilityBase;
class USoundCue;
class ACPP_DamageActor;

struct FDamageReceipt;

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
	virtual void InitEquipmentInfo(const FName& itemID) {};
	virtual void Equip(USceneComponent* Inparent, const FName& SocketName);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CalculateDamage(FDamageReceipt& receipt);

};
