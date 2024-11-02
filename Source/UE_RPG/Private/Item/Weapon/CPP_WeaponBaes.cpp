#include "Item/Weapon/CPP_WeaponBaes.h"

#include "Kismet/GameplayStatics.h"
#include "Widget/NPC/CPP_DamageActor.h"
#include "Item/Item.h"
#include "Sound/SoundCue.h"

ACPP_WeaponBaes::ACPP_WeaponBaes()
{

}

void ACPP_WeaponBaes::UpdateWeaponInfo(FName itemInfoID)
{
	if (IsValid(ItemDataTable))
	{
		const FItemInfoTable* thisItemInfo = ItemDataTable->FindRow<FItemInfoTable>(itemInfoID, TEXT(""));
		if (thisItemInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] was not found!! Please check the ID."), *itemInfoID.ToString());
			return;
		}

		ItemRef = NewObject<UItem>(this, UItem::StaticClass());

		ItemRef->ItemInfoTable = thisItemInfo;

		/**item data*/
		ItemRef->ItemInfoID = thisItemInfo->ItemInfoID;
		ItemRef->Name = thisItemInfo->Name;
		ItemRef->Description = thisItemInfo->Description;
		ItemRef->bCanBeUsed = thisItemInfo->bCanBeUsed;
		ItemRef->bCanStacked = thisItemInfo->bCanStacked;
		ItemRef->UseText = thisItemInfo->UseText;
		ItemRef->Interaction = thisItemInfo->Interaction;
		ItemRef->ItemPrice = thisItemInfo->ItemPrice;
		ItemRef->Weight = thisItemInfo->Weight;
		ItemRef->ATK = thisItemInfo->ATK;

		/**itemtype data*/
		ItemRef->ItemType = thisItemInfo->ItemType;
		ItemRef->ConsumeValue = thisItemInfo->ConsumeValue;
		ItemRef->CombinResultID = thisItemInfo->CombinResultID;
		if (thisItemInfo->ItemClass)
		{
			ItemRef->ItemClass = thisItemInfo->ItemClass;
		}
		ItemRef->WeaponAbilityID = thisItemInfo->WeaponAbilityID;


		/**asset data*/
		ItemRef->ItemMesh = thisItemInfo->ItemMesh;
		ItemRef->IconTexture = thisItemInfo->IconTexture;
		ItemRef->ItemSkeletalMesh = thisItemInfo->ItemSkeletalMesh;
		ItemRef->FireParticle = thisItemInfo->FireParticle;

		WeaponMesh->SetSkeletalMesh(ItemRef->ItemSkeletalMesh);
	}
}

// Called when the game starts or when spawned
void ACPP_WeaponBaes::BeginPlay()
{
	Super::BeginPlay();
	StoreDamageUI();
}

void ACPP_WeaponBaes::SpawnDamageUI(const FVector pos, float damage)
{
	ACPP_DamageActor* damageActor = GetDamageActor();
	damageActor->UpdateDamageActor(pos, damage);
}

void ACPP_WeaponBaes::StoreDamageUI()
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

ACPP_DamageActor* ACPP_WeaponBaes::GetDamageActor()
{
	DamageUI = DamageUI > (DamageUIActors.Num() - 1) ? 0 : DamageUI;
	ACPP_DamageActor* nextUI = DamageUIActors[DamageUI];
	DamageUI++;

	return nextUI;
}

void ACPP_WeaponBaes::Equip(USceneComponent* Inparent, const FName& SocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	if (IsValid(EquipSound))
	{
		UGameplayStatics::PlaySound2D(this, EquipSound);
	}
	WeaponMesh->AttachToComponent(Inparent, TransformRules, SocketName);
}


