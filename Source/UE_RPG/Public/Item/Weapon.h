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

protected:
	virtual void BeginPlay() override;

};
