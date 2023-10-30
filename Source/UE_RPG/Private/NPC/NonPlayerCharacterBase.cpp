#include "NPC/NonPlayerCharacterBase.h"
#include "NPC/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animations/CPP_NPCAnimInstance.h"
#include "Object/Mover.h"

ANonPlayerCharacterBase::ANonPlayerCharacterBase()
{
	//PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HPBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	Mover = CreateDefaultSubobject<UMover>(TEXT("Moving component"));
}


void ANonPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	NPCAnimInstance = Cast<UCPP_NPCAnimInstance>(GetMesh()->GetAnimInstance());
	CurrentHP = MaxHealth;

	UpdateHealthPercent(CurrentHP);
	
}

float ANonPlayerCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHealth);
	UpdateHealthPercent(CurrentHP);

	if (CurrentHP <= 0)
	{
		DieNPC();
	}

	return DamageAmount;
}

void ANonPlayerCharacterBase::UpdateHealthPercent(float currentAmount)
{
	if (!IsValid(HealthBarComponent))
	{
		WARNINGLOG(TEXT("HealthBarComponent is not valid!!"))
		return;
	}
		
	
	float hp = currentAmount / MaxHealth;

	HealthBarComponent->SetHealthPercent(hp);
	
}

void ANonPlayerCharacterBase::DieNPC()
{
	if (!IsValid(DeathActionMontage) || !IsValid(NPCAnimInstance))
		return;
	
	SetStateDeath();
	NPCAnimInstance->Montage_Play(DeathActionMontage);
	
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, TEXT("MoveDown"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 2.f, false);
}

void ANonPlayerCharacterBase::SetStateDeath()
{
	if (IsValid(HealthBarComponent))
		HealthBarComponent->SetVisibility(false);

	CharaterActionState = ECharacterActionState::Death;
	NPCAnimInstance->SetNPCActionState(CharaterActionState);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANonPlayerCharacterBase::MoveDown()
{
	if (!IsValid(Mover))
		return;

	Mover->MoveStart();
}





