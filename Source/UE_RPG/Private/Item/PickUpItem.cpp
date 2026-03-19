#include "Item/PickUpItem.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "Systems/CPP_AkashicSubsystem.h"


APickUpItem::APickUpItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Mesh"));
	SetRootComponent(PickUpMesh);

	SearchComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone item search trace"));
	SearchComponent->SetupAttachment(GetRootComponent());

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone Trace When OverLap"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(180.f);

	ItemStateWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemState Widget"));
	ItemStateWidget->SetupAttachment(GetRootComponent()); 
	

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
	/**Widget*/
	ItemStateWidget->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

void APickUpItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemInfoID != NAME_None)
	{
		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			return;
		}

		UCPP_AkashicSubsystem* AS = World->GetSubsystem<UCPP_AkashicSubsystem>();
		const FItemInfoTable* thisItemInfo = AS->RequestItemInfo(ItemInfoID);
		if (thisItemInfo != nullptr && !thisItemInfo->ItemMesh.IsNull())
			PickUpMesh->SetStaticMesh(thisItemInfo->ItemMesh.LoadSynchronous());
	}
	else
	{
		PickUpMesh->SetStaticMesh(nullptr);
	}
}

void APickUpItem::SetWidgetVisibility(bool Visible)
{
	if(!IsValid(ItemStateWidget))
		return;
	
	ItemStateWidget->SetVisibility(Visible);
}

void APickUpItem::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SphereComponent))
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnSphereOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APickUpItem::OnSphereEndOverlap);
	}

	if (IsValid(ItemStateWidget))
	{
		ItemStateWidget->SetVisibility(false);
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

void APickUpItem::InitializePickUpItem()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (UCPP_AkashicSubsystem* AS = World->GetSubsystem<UCPP_AkashicSubsystem>())
	{
		const FItemInfoTable* thisItemInfo = AS->RequestItemInfo(ItemInfoID);
		if (thisItemInfo)
		{
			ItemRef = NewObject<UItem>(this, UItem::StaticClass());
			ItemRef->ItemInfoTable = *thisItemInfo;

			if (!thisItemInfo->ItemMesh.IsNull())
				PickUpMesh->SetStaticMesh(thisItemInfo->ItemMesh.LoadSynchronous());
		}
	}
}

void APickUpItem::RequestInteract(AActor* interactor)
{
	if (ACPP_Character* character = Cast<ACPP_Character>(interactor))
	{

		if (ItemRef->ItemInfoTable.ItemType == EItemCategory::EIS_Equipment)
		{
			character->PickUpWeapon(ItemRef);
		}
		else
		{
			character->AddInventory(ItemRef, ItemAmount);
		}		
	}

	Destroy();
}

void APickUpItem::OnBeginLookAt()
{
	ItemStateWidget->SetVisibility(true);
}

void APickUpItem::OnEndLookAt()
{
	ItemStateWidget->SetVisibility(false);
}





