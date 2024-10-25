// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/CPP_EnemySpawnArea.h"
#include "Components/SphereComponent.h"

#include "CPP_Character.h"
#include "NPC/EnemyBase.h"

// Sets default values
ACPP_EnemySpawnArea::ACPP_EnemySpawnArea()
{
	SpawnArea = CreateDefaultSubobject<USphereComponent>(TEXT("Respone item search trace"));
	SpawnArea->SetupAttachment(GetRootComponent());

	SpawnArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SpawnArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SpawnArea->SetSphereRadius(SearchAreaRadius);
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
	//CreateEnemy();
	SpawnEnemy();
}

void ACPP_EnemySpawnArea::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* character = Cast<ACPP_Character>(OtherActor);
	if (IsValid(character))
	{
		/**for Encounter function call once*/
		if (Target == character)
			return;
		
		Target = character; 
		Encounter();
	}
}

void ACPP_EnemySpawnArea::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (EnemysNum <= 0)
		TargetIsNotValid();
}

void ACPP_EnemySpawnArea::SpawnEnemy()
{
	if (Enemys.Num() <= 0)
		return;

	EnemysNum = Enemys.Num();

	for (int16 i = 0; i < Enemys.Num(); i++)
	{
		if (!IsValid(Enemys[i]))
			return;

		Enemys[i]->Spawn(this,i);
	}
}

void ACPP_EnemySpawnArea::FocusTarget()
{
	if (!IsValid(Target) || Enemys.Num() <= 0)
	{
		//WARNINGLOG(TEXT("can not work FocusTarget please check valid Target or EnemysNum"))
		return;
	}
		

	FVector targetPos = Target->GetActorLocation();

	float dis = (CenterPos - targetPos).Length();

	if (dis > ValidDis || Target->GetCharacterState() == ECharacterStateTypes::Death)
	{
		WARNINGLOG(TEXT("stop"))
		TargetIsNotValid();
		return;
	}

	{
		UWorld* world = GetWorld();
		if (!IsValid(world))
			return;

		FTimerHandle TimerHandle;
		world->GetTimerManager().SetTimer(TimerHandle, this, &ACPP_EnemySpawnArea::FocusTarget, SPEED1, false);
	}
}

void ACPP_EnemySpawnArea::Encounter()
{
	for (int16 i = 0; i < Enemys.Num(); i++)
	{
		if (!IsValid(Enemys[i]))
			return;

		Enemys[i]->SetTarget(Target);
	}

	FocusTarget();
}

void ACPP_EnemySpawnArea::TargetIsNotValid()
{
	for (int16 i = 0; i < Enemys.Num(); i++)
	{
		if (!IsValid(Enemys[i]))
			return;

		Enemys[i]->IsOrderfromSpawnArea(true);
		Enemys[i]->SetTarget(nullptr);
	}

	Target = nullptr;
}

void ACPP_EnemySpawnArea::EnemyDeathCount(const int32 arrNum)
{
	EnemysNum--;
	if (EnemysNum <= 0)
	{
		FVector targetPos = Target->GetActorLocation();
		float dis = (CenterPos - targetPos).Length();

		if (dis > SearchAreaRadius)
		{
			TargetIsNotValid();
			WARNINGLOG(TEXT("TargetIsNotValid"))
		}
	}
}

void ACPP_EnemySpawnArea::EnemySpawnCount(const int32 arrNum)
{
	EnemysNum++;
}

void ACPP_EnemySpawnArea::CreateEnemy()
{
	UWorld* world = GetWorld();
	for (int32 i = 0; i < EnemyClass.Num(); i++)
	{
		AEnemyBase* enemy = world->SpawnActor<AEnemyBase>(EnemyClass[i]);
		Enemys.Push(enemy);
	}
}


