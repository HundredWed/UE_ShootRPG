#include "Widget/NPC/CPP_DamageActor.h"
#include "NPC/CPP_DamageComponent.h"
#include "Widget/NPC/CPP_DamageUI.h"
#include "Structs/ST_DamageFeedback.h"


ACPP_DamageActor::ACPP_DamageActor()
{
	DamageComponent = CreateDefaultSubobject<UCPP_DamageComponent>(TEXT("Damage UI"));
	SetRootComponent(DamageComponent);
}

void ACPP_DamageActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_DamageActor::UpdateDamageActor(const FDamageReceipt& receipt)
{
	DamageComponent->UpdateDamageUI(receipt.Damage, receipt.DamageType);

	FVector newPoint = receipt.DamagedPoint;
	newPoint.Z += 25.f;
	SetActorLocation(newPoint);	
}

bool ACPP_DamageActor::IsActivateUI()
{
	return DamageComponent->IsActivateUI();
}

