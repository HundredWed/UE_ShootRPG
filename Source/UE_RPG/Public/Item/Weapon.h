#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Weapon.generated.h"

UCLASS()
class UE_RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void Equip(USceneComponent* Inparent, const FName& SocketName);
	void AttachFunc(USceneComponent* Inparent, const FName& SocketName);

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* WeaponMesh;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
