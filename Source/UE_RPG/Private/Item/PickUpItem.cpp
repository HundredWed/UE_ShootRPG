#include "Item/PickUpItem.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory.h"
#include "UE_RPG/MyLogMecro.h"

APickUpItem::APickUpItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Mesh"));
	SetRootComponent(PickUpMesh);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));

	SearchComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone item search trace"));
	SearchComponent->SetupAttachment(GetRootComponent());

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone Trace When OverLap"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(180.f);

	ItemStateWidjet = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemState Widjet"));
	ItemStateWidjet->SetupAttachment(GetRootComponent()); 
	

	/**ECC_GameTraceChannel1 = Grab Trace*/
	/**ECC_GameTraceChannel2 = Gun(hit) Trace*/
	/**ECC_GameTraceChannel3 = Item Search Trace*/
	/**item mesh*/
	PickUpMesh->SetSimulatePhysics(true);
	PickUpMesh->SetEnableGravity(true);
	//StaticMesh->SetVisibility(true);
	PickUpMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	/**search sphere*/
	SearchComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SearchComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
	SearchComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	/**overlap sphere*/
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	/**Widjet*/
	ItemStateWidjet->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

void APickUpItem::SetWidgetVisibility(bool Visible)
{
	if(!IsValid(ItemStateWidjet))
		return;
	
	ItemStateWidjet->SetVisibility(Visible);
}

void APickUpItem::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SphereComponent))
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnSphereOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APickUpItem::OnSphereEndOverlap);
	}

	if (IsValid(ItemStateWidjet))
	{
		ItemStateWidjet->SetVisibility(false);
		bValidWidget = true;
	}

	InitializePickUpItem();
}


void APickUpItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		//DISPLAYLOG(TEXT("Player In!!"));
		character->SetCanSearchObject(true);
		character->OverlapCount += 1;
	}
}

void APickUpItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
     	if (character->OverlapCount > 0)
		{
			character->OverlapCount -= 1;
			//DISPLAYLOG(TEXT("Count Min!!"));
			if (character->OverlapCount == 0)
			{
				character->SetCanSearchObject(false);
				//DISPLAYLOG(TEXT("Player Out!!"));
			}
		}
		
	}
}

void APickUpItem::SetItemState(EItemState Stat)
{
}

void APickUpItem::InitializePickUpItem()
{
	if (IsValid(ItemDataTable))
	{
		const FItemInfoTable* thisItemInfo = ItemDataTable->FindRow<FItemInfoTable>(ItemInfoID, TEXT(""));
		if (thisItemInfo == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] was not found!! Please check the ID."), *ItemInfoID.ToString());
			return;
		}
				
		ItemRef = NewObject<UItem>(this, UItem::StaticClass());

		//ItemRef->ItemInfoTable = thisItemInfo;

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

		if(IsValid(ItemRef->ItemMesh))
			PickUpMesh->SetStaticMesh(ItemRef->ItemMesh);

		if (IsValid(ItemRef->ItemSkeletalMesh))
			WeaponMesh->SetSkeletalMesh(ItemRef->ItemSkeletalMesh);
	}

}

void APickUpItem::TakePickUp(ACPP_Character* taker)
{
	UInventory* playerinventory = taker->GetInventory();

	if (IsValid(playerinventory))
	{
		EItemCategory itemType = ItemRef->ItemType;
		switch (itemType)
		{
		case EItemCategory::EIS_Gabbable:
			return;
		case EItemCategory::EIS_Gold:
			const int32 amountOver = (playerinventory->GetCurrentGold() + (ItemRef->ItemPrice * ItemAmount));
			if (playerinventory->IsOverGold(amountOver))
			{
				return;
			}
			playerinventory->AddGold(ItemRef->ItemPrice * ItemAmount);
			Destroy();
			return;
		}
		playerinventory->AddItem(ItemRef, ItemAmount);
		Destroy();
	}
	
}




