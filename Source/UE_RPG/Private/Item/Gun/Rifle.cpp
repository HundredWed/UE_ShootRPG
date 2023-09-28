#include "Item/Gun/Rifle.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "CPP_Character.h"


ARifle::ARifle()
{
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(WeaponMesh);
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARifle::PullTrigger()
{
	if (IsValid(ShootSound))
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());
	}
	
	FHitResult hitresult;
	FVector hitpoint;

	ViewPointTrace(hitresult, hitpoint);
	GunTrace(hitresult, hitpoint);

	if (IsValid(FireParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, hitpoint, FRotator::ZeroRotator, ParticleSize);
	}
	
	FVector beamspawnpoint = SpawnPoint->GetComponentLocation();
	UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
	if (IsValid(Beam))
	{
		Beam->SetVectorParameter(FName("Target"), hitpoint);
	}
	
}

void ARifle::ViewPointTrace(FHitResult& hitresult, FVector& endpoint)
{
	AController* OwnerController = GetOwnerController();
	if (IsValid(OwnerController) == false)
	{
		return ;
	}
	FVector  location;
	FRotator rotation;

	OwnerController->GetPlayerViewPoint(location, rotation);

	FRotator spreadBullet = rotation;
	SpreadBulletRandomRange(spreadBullet);

	/**between camera and player aiming issue*/
	location = location + rotation.Vector() * TraceStartPoint;

	FVector end = location + spreadBullet.Vector() * MaxDir;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool onhit = GetWorld()->LineTraceSingleByChannel(hitresult, location, end, ECC_GameTraceChannel2, Params);
	//DrawDebugLine(GetWorld(), location, end, FColor::Blue, false, 5); 

	if (onhit)
	{
		endpoint = hitresult.ImpactPoint;
	}
	else
	{
		endpoint = end;
	}

}

void ARifle::GunTrace(FHitResult& hitresult, FVector& endpoint)
{
	FVector StartLocation = SpawnPoint->GetComponentLocation();
	FVector end = endpoint;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), StartLocation, end, FColor::Red, false, 5);

	bool onhit = GetWorld()->LineTraceSingleByChannel(hitresult, StartLocation, end, ECC_GameTraceChannel2, Params);
	if (onhit)
	{
		endpoint = hitresult.ImpactPoint;
	}
}



void ARifle::SpreadBulletRandomRange(FRotator& randDir)
{
	float spreadrange = 1.f;

	ACPP_Character* character = Cast<ACPP_Character>(GetOwner());
	if (IsValid(character))
	{
		spreadrange = character->GetCrosshairSpreadMultiplier() + 0.2f;
		//UE_LOG(LogTemp, Warning, TEXT("spreadrange!!"));
	}

	float randPich = FMath::FRandRange(-spreadrange, spreadrange);
	float randYaw = FMath::FRandRange(-spreadrange, spreadrange);
	float randRoll = FMath::FRandRange(-spreadrange, spreadrange);

	randDir += FRotator(randPich, randYaw, randRoll);
}




AController* ARifle::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (IsValid(OwnerPawn) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn null!!"));
		return nullptr;
	}
		

	return OwnerPawn->GetController();
	
}
