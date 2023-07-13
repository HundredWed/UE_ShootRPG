#include "Item/Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
