#include "Item/PickUpItem.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory.h"

APickUpItem::APickUpItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Mesh"));
	SetRootComponent(PickUpMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone Trace When OverLap"));
	SphereComponent->SetupAttachment(GetRootComponent());

	ItemStateWidjet = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemState Widjet"));
	ItemStateWidjet->SetupAttachment(GetRootComponent()); 
	


	/**ECC_Visibility = Item Search Trace*/
	/**ECC_GameTraceChannel1 = Grab Trace*/
	/**ECC_GameTraceChannel2 = Gun(hit) Trace*/
	/**item mesh*/
	PickUpMesh->SetSimulatePhysics(true);
	PickUpMesh->SetEnableGravity(true);
	//StaticMesh->SetVisibility(true);
	PickUpMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	PickUpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	/**overlap sphere*/
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
	}

	InitializePickUpItem();
}


void APickUpItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		//UE_LOG(LogTemp, Display, TEXT("Player In!!"));
		character->SetCanSearchObject(true);
	}
}

void APickUpItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		character->SetCanSearchObject(false);
	}
}

void APickUpItem::SetItemState(EItemState Stat)
{
}

void APickUpItem::InitializePickUpItem()
{
	if (IsValid(ItemDataTable))
	{
		const FItemInfo* thisItemInfo = ItemDataTable->FindRow<FItemInfo>(ItemInfoID, TEXT(""));

		ItemRef = NewObject<UItem>(this, UItem::StaticClass());

		ItemRef->ItemInfoID = thisItemInfo->ItemInfoID;
		ItemRef->Name = thisItemInfo->Name;
		ItemRef->ItemMesh = thisItemInfo->ItemMesh;
		ItemRef->Description = thisItemInfo->Description;
		ItemRef->IconTexture = thisItemInfo->IconTexture;
		ItemRef->bCanBeUsed = thisItemInfo->bCanBeUsed;
		ItemRef->bCanStacked = thisItemInfo->bCanStacked;
		ItemRef->UseText = thisItemInfo->UseText;
		ItemRef->Interaction = thisItemInfo->Interaction;
		ItemRef->ItemPrice = thisItemInfo->ItemPrice;
		ItemRef->ItemType = thisItemInfo->ItemType;
		ItemRef->ATK = thisItemInfo->ATK;

		PickUpMesh->SetStaticMesh(ItemRef->ItemMesh);
	}

}

void APickUpItem::TakePickUp(ACPP_Character* taker)
{
	UInventory* playerinventory = taker->GetInventory();

	if (IsValid(playerinventory) && ItemRef->ItemType != EItemCategory::EIS_Gabbable)
	{
		playerinventory->AddItem(ItemRef, ItemAmount);
		Destroy();
	}
	
}




