#include "Item/Gun/ShootGun.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


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

	FVector beamspawnpoint = SpawnPoint->GetComponentLocation();
	UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
	if (IsValid(Beam))
	{
		Beam->SetVectorParameter(FName("Target"), HitResult.ImpactPoint);
	}
}

bool AShootGun::GunTrace(FHitResult& hitresult)
{
	AController* OwnerController = GetOwnerController();
	if (IsValid(OwnerController) == false)
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
	if (IsValid(OwnerPawn) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn null!!"));
		return nullptr;
	}
		

	return OwnerPawn->GetController();
	
}
