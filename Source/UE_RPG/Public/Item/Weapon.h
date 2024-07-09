#pragma once

#include "CoreMinimal.h"
#include "Item/PickUpItem.h"
#include "UE_RPG/UtilityMecro.h"
#include "Weapon.generated.h"

UCLASS()
class UE_RPG_API AWeapon : public APickUpItem
{
	GENERATED_BODY()

public:
	AWeapon();
	
	UPROPERTY(EditAnywhere)
		class USoundCue* PickUpSound;

	UPROPERTY(EditAnywhere)
		class USoundCue* EquipSound;

	UPROPERTY(EditAnywhere, Category = "FireEffect")
		class USoundCue* ShootSound;

	UPROPERTY(EditAnywhere, Category = "FireEffect")
		class UParticleSystem* BeamParticle;

	UPROPERTY(EditAnywhere, Category = "FireEffect")
		FVector ParticleSize;

	UPROPERTY(EditAnywhere, Category = "FireEffect")
		class UParticleSystem* FireParticle;


protected:
	virtual void BeginPlay() override;
	//virtual void Tick(float deltatime) override;

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

	/**damage ui*/
	void SpawnDamageUI(const FVector pos, float damage = 0.f);

	/**weapon states*/
	bool bActiveWeapon = true;
	float FinalDamage = 0;

public:
	void Equip(USceneComponent* Inparent, const FName& SocketName);
	void AttachFunc(USceneComponent* Inparent, const FName& SocketName);

	void SetActiveWeapon(bool bactive);
	bool IsActiveWaepon();

	virtual void SetItemState(EItemState State) override;
	void InitializeWeapon();
	void UpdateFinalDamage();
};
