#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Respone Trace When OverLap"));
	SphereComponent->SetupAttachment(GetRootComponent());

	ItemStateWidjet = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemState Widjet"));
	ItemStateWidjet->SetupAttachment(GetRootComponent());
	
}

void AItem::SetWidgeVisibility(bool Visible)
{
	ItemStateWidjet->SetVisibility(Visible);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SphereComponent))
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	}
	
	if (IsValid(ItemStateWidjet))
	{
		ItemStateWidjet->SetVisibility(false);
	}
	
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		character->SetCanSearchObject(true);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		character->SetCanSearchObject(false);
	}
}

void AItem::SetItemState(EItemState State)
{
	switch (State)
	{
	case EItemState::EIS_UnEquipped:
		/**item mesh*/
		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetEnableGravity(false);
		//StaticMesh->SetVisibility(true);
		StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	case EItemState::EIS_Equipped:
		/**item mesh*/
		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetEnableGravity(false);
		//StaticMesh->SetVisibility(true);
		StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}


