#include "Item/Weapon/CPP_EquipmentBase.h"

#include "Kismet/GameplayStatics.h"
#include "Widget/NPC/CPP_DamageActor.h"
#include "Sound/SoundCue.h"
#include "Systems/CPP_AkashicSubsystem.h"

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

void ACPP_EquipmentBase::SpawnDamageUI(const FVector pos, float damage)
{
	ACPP_DamageActor* damageActor = GetDamageActor();
	damageActor->UpdateDamageActor(pos, damage);
}

void ACPP_EquipmentBase::StoreDamageUI(TSubclassOf<ACPP_DamageActor> damageUIActorClass)
{
	UWorld* world = GetWorld();
	const int32 amount = 50;

	if (IsValid(world) && IsValid(damageUIActorClass))
	{
		for (int32 i = 0; i < amount; i++)
		{
			ACPP_DamageActor* damageActor = world->SpawnActor<ACPP_DamageActor>(damageUIActorClass);
			DamageUIActors.Push(damageActor);
		}
	}
	else
	{
		WARNINGLOG(TEXT("is not valid DamageUIActorClass!!"))
	}
}

ACPP_DamageActor* ACPP_EquipmentBase::GetDamageActor()
{
	DamageUI = DamageUI > (DamageUIActors.Num() - 1) ? 0 : DamageUI;
	ACPP_DamageActor* nextUI = DamageUIActors[DamageUI];
	DamageUI++;

	return nextUI;
}

void ACPP_EquipmentBase::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}


