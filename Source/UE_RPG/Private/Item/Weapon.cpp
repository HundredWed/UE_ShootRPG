#include "Item/Weapon.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMesh);

	SphereComponent->SetupAttachment(GetRootComponent());
	ItemStateWidjet->SetupAttachment(GetRootComponent());

}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->DestroyComponent();

	SetItemState(EItemState::EIS_UnEquipped);
}
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}


void AWeapon::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	AttachFunc(Inparent, SocketName);
	SetItemState(EItemState::EIS_Equipped);
	ItemStateWidjet->SetVisibility(false);
}

void AWeapon::AttachFunc(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}

void AWeapon::SetItemState(EItemState State)
{
	switch (State)
	{
		/**ECC_Visibility = Item Search Trace*/
		/**ECC_GameTraceChannel1 = Grab Trace*/
		/**ECC_GameTraceChannel2 = Gun(hit) Trace*/

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