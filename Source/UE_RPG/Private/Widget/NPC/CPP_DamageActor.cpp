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
	if (!IsValid(DamageComponent))
		return;

	SetActorHiddenInGame(true);
}

void ACPP_DamageActor::UpdateDamageActor(const FVector& pos, const int32 amount)
{
	SetActorHiddenInGame(false);
	DamageComponent->UpdateDamageUI(amount);
	SetActorLocation(pos);

	StartValidTime();
}

void ACPP_DamageActor::StartValidTime()
{
	UWorld* world = GetWorld();
	if (!IsValid(world))
		return;

	world->GetTimerManager().SetTimer(TimerHandle, this, &ACPP_DamageActor::SetActorHidden, 0.6f, false);
}

void ACPP_DamageActor::SetActorHidden()
{
	SetActorHiddenInGame(true);
}

