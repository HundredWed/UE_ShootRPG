#include "Item/Gun/ShootGun.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "CPP_Character.h"


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
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, HitResult.ImpactPoint, FRotator::ZeroRotator/*,ParticleSize*/);
		//UE_LOG(LogTemp, Warning, TEXT("Hit!!"));

		FVector beamspawnpoint = SpawnPoint->GetComponentLocation();
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
		if (IsValid(Beam))
		{
			Beam->SetVectorParameter(FName("Target"), HitResult.ImpactPoint);
		}

		/**ondamege...*/
	}
	else
	{
		FVector beamspawnpoint = SpawnPoint->GetComponentLocation();
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
		if (IsValid(Beam))
		{
			Beam->SetVectorParameter(FName("Target"), NoHitLocation);
		}
	}

	
}

bool AShootGun::GunTrace(FHitResult& hitresult)
{
	FVector StartLocation = SpawnPoint->GetComponentLocation();
	FVector end = GetHitPointDirection();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	DrawDebugLine(GetWorld(), StartLocation, end, FColor::Red, false, 5);

	return GetWorld()->LineTraceSingleByChannel(hitresult, StartLocation, end, ECC_GameTraceChannel2, Params);
}

FVector AShootGun::GetHitPointDirection()
{
	AController* OwnerController = GetOwnerController();
	if (IsValid(OwnerController) == false)
	{
		return FVector::Zero();
	}
	FVector  location;
	FRotator rotation;

	OwnerController->GetPlayerViewPoint(location, rotation);

	FRotator spreadBullet = rotation;
	SpreadBulletRandomRange(spreadBullet);

	/**between camera and player aiming issue*/
	location = location + rotation.Vector() * TraceStartPoint;

	FVector end = location + spreadBullet.Vector() * MaxDir;
	NoHitLocation = end;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	FHitResult hitresult;
	bool onhit = GetWorld()->LineTraceSingleByChannel(hitresult, location, end, ECC_GameTraceChannel2, Params);
	DrawDebugLine(GetWorld(), location, end, FColor::Blue, false, 5);

	if (onhit)
	{
		return hitresult.ImpactPoint;
	}

	return NoHitLocation;
}

void AShootGun::SpreadBulletRandomRange(FRotator& randDir)
{
	float spreadrange = 1.f;

	ACPP_Character* character = Cast<ACPP_Character>(GetOwner());
	if (IsValid(character))
	{
		spreadrange = character->GetCrosshairSpreadMultiplier();
		//UE_LOG(LogTemp, Warning, TEXT("spreadrange!!"));
	}

	float randPich = FMath::FRandRange(-spreadrange, spreadrange);
	float randYaw = FMath::FRandRange(-spreadrange, spreadrange);
	float randRoll = FMath::FRandRange(-spreadrange, spreadrange);

	randDir += FRotator(randPich, randYaw, randRoll);
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
