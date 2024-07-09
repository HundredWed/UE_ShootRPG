#include "NPC/CPP_Enemy_Gunner.h"
#include "CPP_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ACPP_Enemy_Gunner::ACPP_Enemy_Gunner()
{
	TurnSpeed = 3.f;
}

void ACPP_Enemy_Gunner::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Enemy_Gunner::Combat()
{
	if (!IsValid(Target))
		return;

	float animLength = 0.f;

	if (PlaySection.Num() > 0)
	{
		int32 random = FMath::RandRange(0, PlaySection.Num()-1);
		/*if (random == PlaySection.Num())
		{
			BehaviorMode(NPCState = ENPCState::SideStep);
			return;
		}*/
		animLength = PlayNPCMontage(CombatActionMontage, PlaySection[random]);
	}
	else
	{
		animLength = PlayNPCMontage(CombatActionMontage);
	}

	if (bDirectly)
	{
		const float combatSpeed = 2.f;
		animLength /= combatSpeed;
		bDirectly = false;
	}
	LookAtTarget(Target->GetActorLocation());
	GunTrace();

	if (CanUpdateState() && animLength > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::UpdateState, animLength, false);
	}
}

void ACPP_Enemy_Gunner::GunTrace()
{
	FVector StartLocation = GetMesh()->GetSocketLocation("Muzzle_Front");
	//FVector end = StartLocation + GetActorForwardVector().GetSafeNormal() * ValidSightDis;
	FVector end = StartLocation + (Target->GetActorLocation() - StartLocation).GetSafeNormal() * ValidSightDis;

	float spreadrange = 100.f;
	float randPich = FMath::FRandRange(-spreadrange, spreadrange);
	float randYaw = FMath::FRandRange(-spreadrange, spreadrange);
	float randRoll = FMath::FRandRange(-spreadrange, spreadrange);

	FVector fixEnd{ end.X + randPich, end.Y + randYaw, end.Z + randRoll + spreadrange };

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	//DrawDebugLine(GetWorld(), StartLocation, fixEnd, FColor::Red, false, 5.f); 

	FHitResult hitresult_gunTrace;
	bool onhit = GetWorld()->LineTraceSingleByChannel(hitresult_gunTrace, StartLocation, fixEnd, ECC_GameTraceChannel5, Params);
	if (onhit)
	{
		UGameplayStatics::ApplyDamage(hitresult_gunTrace.GetActor(), ATK, GetController(), GetOwner(), UDamageType::StaticClass());

		FVector beamspawnpoint = StartLocation;
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
		if (IsValid(Beam))
		{
			Beam->SetVectorParameter(FName("Target"), hitresult_gunTrace.ImpactPoint);
		}
	}
	else
	{
		FVector beamspawnpoint = StartLocation;
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(this, BeamParticle, beamspawnpoint);
		if (IsValid(Beam))
		{
			Beam->SetVectorParameter(FName("Target"), (fixEnd - StartLocation) * 1000.f);
		}
	}

	if (IsValid(FireParticle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, FireParticle, StartLocation, FRotator::ZeroRotator, ParticleSize);
	}
}
