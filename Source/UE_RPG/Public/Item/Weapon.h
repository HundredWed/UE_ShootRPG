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

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult) override;

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex) override;

	/**weapon states*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon item states", meta = (AllowPrivateAccess = "true"))
		int32 ATK = 0;
public:

	virtual void SetItemState(EItemState State) override;
private:

	
};
