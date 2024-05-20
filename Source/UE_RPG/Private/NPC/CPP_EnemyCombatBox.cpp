#include "NPC/CPP_EnemyCombatBox.h"
#include "Components/BoxComponent.h"

ACPP_EnemyCombatBox::ACPP_EnemyCombatBox()
{
	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Box"));
	SetRootComponent(CombatBox);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ACPP_EnemyCombatBox::BeginPlay()
{
	Super::BeginPlay();
	
}
