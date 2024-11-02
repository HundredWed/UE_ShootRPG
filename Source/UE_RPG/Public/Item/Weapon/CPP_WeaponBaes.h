#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemData.h"
#include "Engine/DataTable.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_WeaponBaes.generated.h"

UCLASS()
class UE_RPG_API ACPP_WeaponBaes : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPP_WeaponBaes();

	UPROPERTY(VisibleAnywhere, Category = "WeaponInfo")
		class USoundCue* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "WeaponInfo")
		class USoundCue* AttackSound;

	UPROPERTY(VisibleAnywhere, Category = "WeaponInfo")
		USkeletalMeshComponent* WeaponMesh;

	/**widget*/
	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<class ACPP_DamageActor> DamageUIActorClass;
	UPROPERTY()
		TArray<class ACPP_DamageActor*> DamageUIActors;
	
		int32 DamageUI = 0;

public:
	//virtual void Tick(float DeltaTime) override;
	void UpdateWeaponInfo(FName itemInfoID);
	virtual void Attack() {};
	void Equip(USceneComponent* Inparent, const FName& SocketName);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**damage ui*/
	void SpawnDamageUI(const FVector pos, float damage = 0.f);
	void StoreDamageUI();
	class ACPP_DamageActor* GetDamageActor();

	/**weapon states*/
	float FinalDamage = 0;

	/**item state*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo|ItemInfo")
		class UItem* ItemRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo|ItemInfo")
		UDataTable* ItemDataTable;
};
