#include "Item/Weapon.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	SetRootComponent(WeaponMesh);

	SphereComponent->SetupAttachment(GetRootComponent());
	ItemStateWidjet->SetupAttachment(GetRootComponent());

	StaticMesh->DestroyComponent();
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	ItemStateWidjet->SetVisibility(true);
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
	ItemState = EItemState::EIS_Equipped;
}

void AWeapon::AttachFunc(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}
