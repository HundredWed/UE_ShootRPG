#include "Item/Weapon.h"

AWeapon::AWeapon()
{
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
void AWeapon::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	AttachFunc(Inparent, SocketName);
	ItemState = EItemState::EIS_Equipped;
}

void AWeapon::AttachFunc(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}