#include "Item/Weapon/CPP_EquipmentBase.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Systems/CPP_AkashicSubsystem.h"
#include "CPP_Character.h"
#include "Structs/ST_DamageFeedback.h"

ACPP_EquipmentBase::ACPP_EquipmentBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void ACPP_EquipmentBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_EquipmentBase::CalculateDamage(FDamageReceipt& receipt)
{
	if (ACPP_Character* owner = Cast<ACPP_Character>(GetOwner()))
	{
		owner->CalculateDamage(receipt);
	}
}

void ACPP_EquipmentBase::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}


