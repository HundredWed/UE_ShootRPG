#include "NPC/CPP_EnemyCombatBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "CPP_Character.h"

#define OVERHIT 1

ACPP_EnemyCombatBox::ACPP_EnemyCombatBox()
{
	CombatBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Box"));
	SetRootComponent(CombatBox);

	/**ECC_GameTraceChannel4 = Player*/
	CombatBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap);
}

void ACPP_EnemyCombatBox::BeginPlay()
{
	Super::BeginPlay();
	CombatBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_EnemyCombatBox::OnBoxOverlap);
}

AController* ACPP_EnemyCombatBox::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwnerPawn))
	{
		return nullptr;
	}


	return OwnerPawn->GetController();
}

void ACPP_EnemyCombatBox::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/**overlap event tiwce issue*/
	{
		Hicount++;

		if (Hicount > OVERHIT)
		{
			//WARNINGLOG(TEXT("OverHit!!!"))
			return;
		}
		FTimerHandle TimeHandle;
		GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &ACPP_EnemyCombatBox::InitCount, DELAY1, false);
	}

	Player = Cast<ACPP_Character>(OtherActor);
	
	if (IsValid(Player))
	{
		
		if (IsValid(HitSound) && (Player->GetActionState() != ECharacterActionState::SuperAction))
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}

		if (bKnockBack)
		{
			FVector knockBackDir = GetOwner()->GetActorForwardVector();
			knockBackDir *= KnockBackVelocity;
			Player->KnockBack(knockBackDir);

			UGameplayStatics::ApplyDamage(Player, Damage, GetOwnerController(), GetOwner(), UDamageType::StaticClass());
		}
		else
		{
			const float damage = 1.f;
			UGameplayStatics::ApplyDamage(Player, damage, GetOwnerController(), GetOwner(), UDamageType::StaticClass());
		}
	}

}

void ACPP_EnemyCombatBox::SetCombatBoxCollisionEnabled(ECollisionEnabled::Type newType)
{
	CombatBox->SetCollisionEnabled(newType);
}
