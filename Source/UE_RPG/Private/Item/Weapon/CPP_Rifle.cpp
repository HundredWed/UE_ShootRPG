// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/CPP_Rifle.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

#include "NPC/EnemyBase.h"
#include "CPP_Character.h"

ACPP_Rifle::ACPP_Rifle()
{
}

void ACPP_Rifle::Attack()
{
	SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Boom!!!!!!!"));
	if (IsValid(ShootSound))
	{
		UGameplayStatics::PlaySound2D(this, ShootSound);
	}

	FHitResult hitresult;
	FVector hitpotin;

	ViewPointTrace(hitresult, hitpotin);
	GunTrace(hitresult, hitpotin);

	TakeHit(hitresult, WeaponMesh->GetSocketLocation("FirePoint"));

	ShootEffect(hitpotin);

}

void ACPP_Rifle::ViewPointTrace(FHitResult& hitresult, FVector& endpoint)
{
	AController* OwnerController = GetOwnerController();
	if (!IsValid(OwnerController))
	{
		return;
	}
	FVector  location;
	FRotator rotation;

	OwnerController->GetPlayerViewPoint(location, rotation);

	FRotator spreadBullet = rotation;
	SpreadBulletRandomRange(spreadBullet);

	/**between camera and player aiming issue*/
	location = location + rotation.Vector() * TraceStartPoint;

	FVector end = location + spreadBullet.Vector() * MaxDis;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool onhit = GetWorld()->LineTraceSingleByChannel(hitresult, location, end, ECC_GameTraceChannel2, Params);
	//DrawDebugLine(GetWorld(), location, end, FColor::Blue, false, 5); 
	if (onhit)
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, TEXT("hit actor"));
		endpoint = hitresult.ImpactPoint;
	}
	else
	{
		endpoint = end;
	}
}

void ACPP_Rifle::GunTrace(FHitResult& hitresult, FVector& endpoint)
{
	FVector StartLocation = WeaponMesh->GetSocketLocation("FirePoint");
	FVector end = endpoint;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	DrawDebugLine(GetWorld(), StartLocation, end, FColor::Red, false, 5);


	FHitResult hitresult_gunTrace;
	bool onhit = GetWorld()->LineTraceSingleByChannel(hitresult_gunTrace, StartLocation, end, ECC_GameTraceChannel2, Params);
	if (onhit)
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, TEXT("hit actor"));
		endpoint = hitresult_gunTrace.ImpactPoint;
		hitresult = hitresult_gunTrace;
	}
}

void ACPP_Rifle::ShootEffect(const FVector& shootpoint)
{
	if (IsValid(FireParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, shootpoint, FRotator::ZeroRotator, ParticleSize);
	}

	FVector beamspawnpoint = WeaponMesh->GetSocketLocation("FirePoint");
	UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
	if (IsValid(Beam))
	{
		Beam->SetVectorParameter(FName("Target"), shootpoint);
	}
}

void ACPP_Rifle::TakeHit(FHitResult& hitresult, const FVector& shootpoint)
{
	AEnemyBase* enemy = Cast<AEnemyBase>(hitresult.GetActor());
	if (!IsValid(enemy))
		return;

	bool damaged = enemy->GetHit(shootpoint);
	if (damaged)
	{
		UGameplayStatics::ApplyDamage(enemy, FinalDamage, GetOwnerController(), GetOwner(), UDamageType::StaticClass());
		SpawnDamageUI(hitresult.ImpactPoint, FinalDamage);
	}
	else if (!damaged)
	{
		SpawnDamageUI(hitresult.ImpactPoint);
	}
}

void ACPP_Rifle::SpreadBulletRandomRange(FRotator& randDir)
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

AController* ACPP_Rifle::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (IsValid(OwnerPawn) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerPawn null!!"));
		return nullptr;
	}


	return OwnerPawn->GetController();
}

void ACPP_Rifle::BeginPlay()
{
	Super::BeginPlay();
}
