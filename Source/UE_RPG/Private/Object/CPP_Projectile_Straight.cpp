#include "Object/CPP_Projectile_Straight.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "CPP_Character.h"

ACPP_Projectile_Straight::ACPP_Projectile_Straight()
{
	DamageEvent = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Collision"));
	DamageEvent->SetupAttachment(GetRootComponent());

	PTComponent->InitialSpeed = 0.f;
	PTComponent->MaxSpeed = 0.f;
}

void ACPP_Projectile_Straight::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(DamageEvent))
		DamageEvent->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Projectile_Straight::OnSphereOverlap);
}

void ACPP_Projectile_Straight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector forward = GetActorForwardVector().GetSafeNormal();
	FVector pos = GetActorLocation();
	FVector newPos = pos + (forward * Speed * DeltaTime);
	SetActorLocation(newPos);
}

void ACPP_Projectile_Straight::ShootProjectile(bool bAEO)
{
	SetActorTickEnabled(true);
}

void ACPP_Projectile_Straight::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* player = Cast<ACPP_Character>(OtherActor);
	if (Hitcount == 0 && IsValid(player) && HitParticles.Num() > 0)
	{
		for (int32 i = 0; i < HitParticles.Num(); i++)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles[i], GetActorLocation(), FRotator::ZeroRotator, HitParticleSize);
		}
		
		UGameplayStatics::ApplyDamage(player, Damage, nullptr, GetOwner(), UDamageType::StaticClass());
		Hitcount++;
	}
}
