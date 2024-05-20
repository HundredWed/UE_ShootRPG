#include "Item/Weapon.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Sound/SoundCue.h"

#include "Item/Item.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Character.h"
#include "Widget/NPC/CPP_DamageActor.h"

#define DAMAGERATIO 15
#define NODATA 0

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(WeaponMesh);

	SphereComponent->SetupAttachment(GetRootComponent());
	ItemStateWidjet->SetupAttachment(GetRootComponent());
	SearchComponent->SetupAttachment(GetRootComponent());
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickUpMesh->DestroyComponent();

	if (IsValid(ItemRef))
	{
		FinalDamage = ItemRef->ATK;
	}
	else
	{
		WARNINGLOG(TEXT("not valid weapon ItemRef"));
	}

	SetItemState(EItemState::EIS_UnEquipped);
	SetActorTickEnabled(false);
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::SpawnDamageUI(const FVector pos, float damage)
{
	ACPP_Character* character = Cast<ACPP_Character>(GetOwner());
	if (IsValid(character))
	{
		ACPP_DamageActor* damageActor = character->GetDamageActor();
		damageActor->UpdateDamageActor(pos, damage);
	}
}

void AWeapon::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	AttachFunc(Inparent, SocketName);
	SetItemState(EItemState::EIS_Equipped);
	UpdateFinalDamage();
	ItemStateWidjet->SetVisibility(false);

	if (IsValid(PickUpSound))
	{
		UGameplayStatics::PlaySound2D(this, PickUpSound);
	}
}

void AWeapon::AttachFunc(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	if (IsValid(EquipSound))
	{
		UGameplayStatics::PlaySound2D(this, EquipSound);
	}
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}

void AWeapon::SetActiveWeapon(bool bactive)
{
	bActiveWeapon = bactive;
	SetActorHiddenInGame(!bactive);
}

bool AWeapon::IsActiveWaepon()
{
	return bActiveWeapon;
}

void AWeapon::SetItemState(EItemState State)
{
	switch (State)
	{
		/**ECC_GameTraceChannel1 = Grab Trace*/
		/**ECC_GameTraceChannel2 = Gun(hit) Trace*/
		/**ECC_GameTraceChannel3 = Item Search Trace*/

	case EItemState::EIS_UnEquipped:
		/**item mesh*/
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		//StaticMesh->SetVisibility(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 

		break;
	case EItemState::EIS_Equipped:
		/**item mesh*/
		WeaponMesh->SetSimulatePhysics(false);
		WeaponMesh->SetEnableGravity(false);
		//StaticMesh->SetVisibility(true);
		WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::InitializeWeapon()
{
	InitializePickUpItem();

	if (ItemRef && ItemRef->FireParticle)
	{
		FireParticle = ItemRef->FireParticle;

		/**ShootSound, BeamParticle, ParticleSize*/
	}
}

void AWeapon::UpdateFinalDamage()
{
	if (!IsValid(ItemRef))
		return;

	ACPP_Character* character = Cast<ACPP_Character>(GetOwner());

	if (IsValid(character)) 
	{
		FinalDamage = (FinalDamage + character->GetPlayerATK()) / DAMAGERATIO;
	}
}
