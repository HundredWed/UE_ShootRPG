#include "Item/PickUpItem.h"
#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

APickUpItem::APickUpItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pick Mesh"));
	SetRootComponent(PickUpMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone Trace When OverLap"));
	SphereComponent->SetupAttachment(GetRootComponent());

	ItemStateWidjet = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemState Widjet"));
	ItemStateWidjet->SetupAttachment(GetRootComponent()); 

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

void APickUpItem::TakePicUp(ACPP_Character* taker)
{
}




