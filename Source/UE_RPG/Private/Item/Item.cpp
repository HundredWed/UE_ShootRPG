#include "Item/Item.h"
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


void AItem::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
