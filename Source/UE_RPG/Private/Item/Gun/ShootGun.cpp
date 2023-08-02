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
	FVector End;

	bool OnHit = GunTrace(HitResult,End);

	if (OnHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, HitResult.ImpactPoint,FRotator::ZeroRotator, ParticleSize);
		//UE_LOG(LogTemp, Warning, TEXT("Hit!!"));

		FVector beamspawnpoint = SpawnPoint->GetComponentLocation();
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
		if (IsValid(Beam))
		{
			Beam->SetVectorParameter(FName("Target"), HitResult.ImpactPoint);
		}
	}
	else
	{
		FVector beamspawnpoint = SpawnPoint->GetComponentLocation();
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
		if (IsValid(Beam))
		{
			Beam->SetVectorParameter(FName("Target"), End);
		}
	}

	
}


bool AShootGun::GunTrace(FHitResult& hitresult, FVector& End)
{
	AController* OwnerController = GetOwnerController();
	if (IsValid(OwnerController) == false)
	{
		return false;
	}

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FRotator spreadBullet = Rotation;

	SpreadBulletRandomRange(spreadBullet);

	End = Location + spreadBullet.Vector() * MaxDir;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 5);

	return GetWorld()->LineTraceSingleByChannel(hitresult, Location, End, ECC_GameTraceChannel2, Params);
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
