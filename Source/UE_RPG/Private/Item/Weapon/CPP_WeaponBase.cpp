#include "Item/Weapon/CPP_WeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Widget/NPC/CPP_DamageActor.h"
#include "Item/Item.h"
#include "Sound/SoundCue.h"

ACPP_WeaponBase::ACPP_WeaponBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(WeaponMesh);
}

void ACPP_WeaponBase::InitWeaponInfo()
{
	if (IsValid(ItemDataTable) && IsValid(EquipmentAssetTable))
	{
		const FItemInfoTable* thisItemInfo = ItemDataTable->FindRow<FItemInfoTable>(ItemInfoID, TEXT(""));
		if (thisItemInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] was not found!! Please check the ID."), *ItemInfoID.ToString());
			return;
		}

		ItemRef = NewObject<UItem>(this, UItem::StaticClass());

		ItemRef->ItemInfoTable = *thisItemInfo;

		if(thisItemInfo->ItemSkeletalMesh)
			WeaponMesh->SetSkeletalMesh(thisItemInfo->ItemSkeletalMesh);
	}
}

// Called when the game starts or when spawned
void ACPP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	InitWeaponInfo();
	StoreDamageUI();
}

void ACPP_WeaponBase::SpawnDamageUI(const FVector pos, float damage)
{
	ACPP_DamageActor* damageActor = GetDamageActor();
	damageActor->UpdateDamageActor(pos, damage);
}

void ACPP_WeaponBase::StoreDamageUI()
{
	UWorld* world = GetWorld();
	const int32 amount = 50;

	if (IsValid(world) && IsValid(DamageUIActorClass))
	{
		for (int32 i = 0; i < amount; i++)
		{
			ACPP_DamageActor* damageActor = world->SpawnActor<ACPP_DamageActor>(DamageUIActorClass);
			DamageUIActors.Push(damageActor);
		}
	}
	else
	{
		WARNINGLOG(TEXT("is not valid DamageUIActorClass!!"))
	}
}

ACPP_DamageActor* ACPP_WeaponBase::GetDamageActor()
{
	DamageUI = DamageUI > (DamageUIActors.Num() - 1) ? 0 : DamageUI;
	ACPP_DamageActor* nextUI = DamageUIActors[DamageUI];
	DamageUI++;

	return nextUI;
}

void ACPP_WeaponBase::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}


