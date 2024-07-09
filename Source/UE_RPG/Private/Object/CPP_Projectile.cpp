#include "Object/CPP_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

#include "Object/CPP_AOE.h"
#include "CPP_Character.h"

ACPP_Projectile::ACPP_Projectile()
{
	PrimaryActorTick.bCanEverTick = true;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(Projectile);

	PTComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PT Component"));
	
	ProjectileTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	ProjectileTrail->SetupAttachment(GetRootComponent());

	PTComponent->InitialSpeed = 1300.f;
	PTComponent->MaxSpeed = 2000.f;

	SetActorTickEnabled(false);
}

void ACPP_Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_Projectile::ShootProjectile(bool bAEO)
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
	ExplosionLocation = location;
	//WARNINGLOG(TEXT("%s"), *location.ToString())
	//WARNINGLOG(TEXT("%s"), *PredictResult.HitResult.Location.ToString())

	const float fixZ = 0.5f;/**to fix the issue of getting buried in the ground*/
	location.Z += fixZ;

	UWorld* world = GetWorld();
	if (bAEO && IsValid(AOEclass) && IsValid(world))
	{		
		AOE = world->SpawnActor<ACPP_AOE>(AOEclass, location, PredictResult.HitResult.ImpactPoint.Rotation());
		AOE->SetAreaSpeed(ActualHitTime);

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Projectile::Explosion, ActualHitTime, false);
	}
}

void ACPP_Projectile::Explosion()
{
	FHitResult hitresult;
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(this);

	bool hit = UKismetSystemLibrary::SphereTraceSingle(this, ExplosionLocation, ExplosionLocation, ExplosionRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel5), false,
		actorsToIgnore, EDrawDebugTrace::ForDuration, hitresult, true, FLinearColor::Red, FLinearColor::Green, -1.f);
	
	ACPP_Character* player = Cast<ACPP_Character>(hitresult.GetActor());

	if (HitParticles.Num() > 0)
	{
		for (int32 i = 0; i < HitParticles.Num(); i++)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles[i], GetActorLocation(), FRotator::ZeroRotator, HitParticleSize);
		}
	}

	if (IsValid(HitSound))
	{
		UGameplayStatics::PlaySound2D(this, HitSound);
	}

	if (hit && IsValid(player))
	{		
		UGameplayStatics::ApplyDamage(player, Damage, nullptr, GetOwner(), UDamageType::StaticClass());

		FVector knockBackDir = (player->GetActorLocation() - ExplosionLocation).GetSafeNormal();
		knockBackDir *= KnockBackVelocity;
		player->KnockBack(knockBackDir);
	}
}
