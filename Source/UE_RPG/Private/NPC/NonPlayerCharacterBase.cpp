#include "NPC/NonPlayerCharacterBase.h"
#include "NPC/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"

ANonPlayerCharacterBase::ANonPlayerCharacterBase()
{
	//PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBarWidjet = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HPBar"));
	HealthBarWidjet->SetupAttachment(GetRootComponent());
}


void ANonPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}





