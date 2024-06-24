#include "Object/CPP_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Object/CPP_AOE.h"

ACPP_Projectile::ACPP_Projectile()
{
	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(Projectile);

	PTComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PT Component"));

	PTComponent->InitialSpeed = 1300.f;
	PTComponent->MaxSpeed = 2000.f;
}

void ACPP_Projectile::BeginPlay()
{
	Super::BeginPlay();

	ShootProjectile();
}

void ACPP_Projectile::ShootProjectile()
{
	const float velocity = PTComponent->InitialSpeed;
	const float projectileRadius = 1.f;

	FVector forward = GetActorForwardVector();
	FVector launch = FVector{ forward.X * velocity, forward.Y * velocity, forward.Z * velocity };

	FPredictProjectilePathResult PredictResult;
	FPredictProjectilePathParams Params = FPredictProjectilePathParams(projectileRadius, GetActorLocation(), launch, 5.f);

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Push(this);

	Params.bTraceWithCollision = true;
	Params.bTraceComplex = false;
	Params.ActorsToIgnore = actorsToIgnore;

	/**debug*/
	//Params.DrawDebugType = EDrawDebugTrace::ForDuration;
	//Params.DrawDebugTime = 2.f;
	//Params.SimFrequency = 20;
	//Params.MaxSimTime = 5.f;

	Params.TraceChannel = ECollisionChannel::ECC_GameTraceChannel6;
	UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictResult);

	const FHitResult& hitResult = PredictResult.HitResult;
	const float ActualHitTime = PredictResult.LastTraceDestination.Time + (PredictResult.LastTraceDestination.Time - PredictResult.PathData.Last().Time) * hitResult.Time;
	//DISPLAYLOG(TEXT("%f"), ActualHitTime)


	FVector location = PredictResult.HitResult.ImpactPoint;
	//WARNINGLOG(TEXT("%s"), *location.ToString())
	//WARNINGLOG(TEXT("%s"), *PredictResult.HitResult.Location.ToString())

	const float fixZ = 0.5f;/**to fix the issue of getting buried in the ground*/
	location.Z += fixZ;

	UWorld* world = GetWorld();
	AOE = world->SpawnActor<ACPP_AOE>(AOEclass, location, PredictResult.HitResult.ImpactPoint.Rotation());
	AOE->SetAreaSpeed(ActualHitTime);
}
