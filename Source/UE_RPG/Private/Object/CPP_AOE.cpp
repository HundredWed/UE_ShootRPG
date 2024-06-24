#include "Object/CPP_AOE.h"

ACPP_AOE::ACPP_AOE()
{
	PrimaryActorTick.bCanEverTick = true;

	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AOE Ring"));
	SetRootComponent(Ring);

	Arae = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AOE Circle"));
	Arae->SetupAttachment(GetRootComponent());

	Ring->SetRelativeScale3D(RingDefaultScale);
	Arae->SetRelativeScale3D(AreaDefaultScale);
}

void ACPP_AOE::BeginPlay()
{
	Super::BeginPlay();	
}

void ACPP_AOE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

 	FVector newArea = Arae->GetRelativeScale3D();
	newArea.X += Speed * DeltaTime;
	newArea.Y += Speed * DeltaTime;
	Arae->SetRelativeScale3D(newArea);

	if (newArea.X >= 1.f)
		SetActorHiddenInGame(true);
}

void ACPP_AOE::SetAreaSpeed(const double time)
{
	Speed = 1.0f / time;
}


