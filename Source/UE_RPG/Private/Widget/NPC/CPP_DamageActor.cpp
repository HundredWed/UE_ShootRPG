#include "Widget/NPC/CPP_DamageActor.h"
#include "NPC/CPP_DamageComponent.h"
#include "Widget/NPC/CPP_DamageUI.h"

ACPP_DamageActor::ACPP_DamageActor()
{
	DamageComponent = CreateDefaultSubobject<UCPP_DamageComponent>(TEXT("Damage UI"));
	SetRootComponent(DamageComponent);
}

void ACPP_DamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_DamageActor::UpdateDamageActor(const FVector& pos, const int32 amount)
{
	DamageComponent->UpdateDamageUI(amount);
	SetActorLocation(pos);
}

