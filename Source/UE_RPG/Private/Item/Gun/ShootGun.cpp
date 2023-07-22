#include "Item/Gun/ShootGun.h"
#include "Kismet/GameplayStatics.h"


AShootGun::AShootGun()
{
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(WeaponMesh);
}

void AShootGun::BeginPlay()
{
	Super::BeginPlay();
}

void AShootGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShootGun::PullTrigger()
{
	UGameplayStatics::PlaySoundAtLocation(this, ShootSound,GetActorLocation());
	

	FHitResult HitResult;

	bool OnHit = GunTrace(HitResult);

	if (OnHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, HitResult.ImpactPoint,FRotator::ZeroRotator, ParticleSize);
		UE_LOG(LogTemp, Warning, TEXT("Hit!!"));
	}

}

bool AShootGun::GunTrace(FHitResult& hitresult)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxDir;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 5);

	return GetWorld()->LineTraceSingleByChannel(hitresult, Location, End, ECC_GameTraceChannel2, Params);
}

AController* AShootGun::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn null!!"));
		return nullptr;
	}
		

	return OwnerPawn->GetController();
	
}
