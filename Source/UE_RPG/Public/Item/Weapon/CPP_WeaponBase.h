#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemData.h"
#include "Engine/DataTable.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_WeaponBase.generated.h"

class UItem;
class UWeaponAbilityBase;

struct FWeaponInfo
{
	/**weapon states*/
	float FinalDamage = 0;
};

UCLASS()
class UE_RPG_API ACPP_WeaponBase : public AActor

{
	GENERATED_BODY()
	
public:	
	ACPP_WeaponBase();

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		class USoundCue* AttackSound;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		USkeletalMeshComponent* WeaponMesh;

	//µð¹ö±ë¿ë
	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		FName ItemInfoID;

	/**widget*/
	UPROPERTY(EditAnywhere, Category = "WeaponInfo")
		TSubclassOf<class ACPP_DamageActor> DamageUIActorClass;
	UPROPERTY()
		TArray<class ACPP_DamageActor*> DamageUIActors;
	
		int32 DamageUI = 0;

		/**weapon states*/
		float FinalDamage = 0;

public:
	//virtual void Tick(float DeltaTime) override;
	virtual void Attack() {};
	void UpdateWeaponInfo(FName itemInfoID);
	void Equip(USceneComponent* Inparent, const FName& SocketName);
	UItem* GetItemRef() { return ItemRef; }
	void SetDataTable(UDataTable* dataTable) { ItemDataTable = dataTable; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**damage ui*/
	void SpawnDamageUI(const FVector pos, float damage = 0.f);
	void StoreDamageUI();
	class ACPP_DamageActor* GetDamageActor();

	/**item state*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponInfo|ItemInfo")
		UItem* ItemRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "WeaponInfo|ItemInfo")
		UDataTable* ItemDataTable;
};
