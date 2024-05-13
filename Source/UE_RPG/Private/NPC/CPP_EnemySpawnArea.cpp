// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/CPP_EnemySpawnArea.h"
#include "Components/SphereComponent.h"

#include "CPP_Character.h"
#include "NPC/EnemyBase.h"

// Sets default values
ACPP_EnemySpawnArea::ACPP_EnemySpawnArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnArea = CreateDefaultSubobject<USphereComponent>(TEXT("Respone item search trace"));
	SpawnArea->SetupAttachment(GetRootComponent());

	SpawnArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SpawnArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	SpawnArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ACPP_EnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(SpawnArea))
	{
		SpawnArea->OnComponentBeginOverlap.AddDynamic(this, &ACPP_EnemySpawnArea::OnSphereOverlap);
		SpawnArea->OnComponentEndOverlap.AddDynamic(this, &ACPP_EnemySpawnArea::OnSphereEndOverlap);

		CenterPos = GetActorLocation();
	}

	SetActorTickEnabled(false);
	SpawnEnemy();
}

void ACPP_EnemySpawnArea::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		Target = character;

		for (int16 i = 0; i < Enemys.Num(); i++)
		{
			if (!IsValid(Enemys[i]))
				return;

			Enemys[i]->SetTarget(Target);
		}
	}
}

void ACPP_EnemySpawnArea::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SetActorTickEnabled(true);
}

// Called every frame
void ACPP_EnemySpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateDis();
}

void ACPP_EnemySpawnArea::SpawnEnemy()
{
	if (Enemys.Num() <= 0)
		return;


	for (int16 i = 0; i < Enemys.Num(); i++)
	{
		if (!IsValid(Enemys[i]))
			return;


		Enemys[i]->Spawn();
	}
}

void ACPP_EnemySpawnArea::CalculateDis()
{
	if (!IsValid(Target))
		return;

	FVector targetPos = Target->GetActorLocation();

	float dis = (CenterPos - targetPos).Length();
	//UE_LOG(LogTemp, Display, TEXT("%f"), dis);

	if (dis < ValidDis)
		return;

	for (int16 i = 0; i < Enemys.Num(); i++)
	{
		if (!IsValid(Enemys[i]))
			return;

		Enemys[i]->IsOrderfromSpawnArea(true);
		Enemys[i]->SetTarget(nullptr);
	}

	SetStateNormal();
}

void ACPP_EnemySpawnArea::SetStateNormal()
{
	SetActorTickEnabled(false);
	Target = nullptr;
}

