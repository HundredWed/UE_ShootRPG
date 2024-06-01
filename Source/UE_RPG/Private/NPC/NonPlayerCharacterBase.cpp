#include "NPC/NonPlayerCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

#include "NPC/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animations/CPP_NPCAnimInstance.h"
#include "Object/Mover.h"
#include "CPP_Character.h"

ANonPlayerCharacterBase::ANonPlayerCharacterBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HPBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	Mover = CreateDefaultSubobject<UMover>(TEXT("Moving component"));
	
	/**set Collision*/
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
	HealthBarComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//CharacterMovement Settting Base 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = 630.f;
	GetCharacterMovement()->RotationRate.Yaw = 180;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}


void ANonPlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	NPCAnimInstance = Cast<UCPP_NPCAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(NPCAnimInstance))
	{
		WARNINGLOG(TEXT("NPCAnimInstance is not valid!!"))
	}
	else
	{
		DISPLAYLOG(TEXT("NPCAnimInstance is nvalid!!"))
	}

	NPCController = Cast<AAIController>(GetController());
	if (!IsValid(NPCController))
	{
		WARNINGLOG(TEXT("Enemy EnemyController is not Valid!!"));
		return;
	}

	if (IsValid(HealthBarComponent))
	{
		CurrentHP = MaxHealth;
		UpdateHealthPercent(CurrentHP);
		SetHealthBarWidget(false);
	}
}

float ANonPlayerCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	SetHealthBarWidget(true);

	CurrentHP = FMath::Clamp(CurrentHP - DamageAmount, 0.f, MaxHealth);
	UpdateHealthPercent(CurrentHP);
	if (CurrentHP <= 0)
	{
		WARNINGLOG(TEXT("DieNPC"))
		DieNPC();
	}

	return DamageAmount;
}

void ANonPlayerCharacterBase::UpdateState()
{
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

	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &ANonPlayerCharacterBase::MoveDown, 
		DeathActionMontage->GetPlayLength() * 2.f, false);
}

void ANonPlayerCharacterBase::SetStateDeath()
{
	StopMove();
	SetHealthBarWidget(false);
	NPCState = ENPCState::Death;
	NPCAnimInstance->SetNPCState(NPCState);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANonPlayerCharacterBase::MoveToActor(const AActor* actor, const int acceptanceRadius)
{
	if (!IsValid(actor))
	{
		WARNINGLOG(TEXT("is not valid actor of MoveToActor Func!!"))
		return;
	}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(actor);
	MoveRequest.SetAcceptanceRadius(acceptanceRadius);
	NPCController->MoveTo(MoveRequest);

	LookAtTarget(actor->GetActorLocation());
}

void ANonPlayerCharacterBase::MoveToLocation(const FVector& pos, const int acceptanceRadius)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(pos);
	MoveRequest.SetAcceptanceRadius(acceptanceRadius);
	NPCController->MoveTo(MoveRequest);
	LookAtTarget(pos);
}

float ANonPlayerCharacterBase::PlayNPCMontage(UAnimMontage* montageToPlay)
{
	if (!IsValid(montageToPlay) || !IsValid(NPCAnimInstance))
	{
		WARNINGLOG(TEXT("is not valid montage or NPCAnimInstance!!"))
		return 0.f;
	}
		
	NPCAnimInstance->Montage_Play(montageToPlay);
	
	return montageToPlay->GetPlayLength() + 0.05f;/**0.05f delay*/
}

float ANonPlayerCharacterBase::CheckDist()
{
	if (!IsValid(Target))
		return -1;

	FVector targetPos = Target->GetActorLocation();
	return (targetPos - GetActorLocation()).Length();
}

void ANonPlayerCharacterBase::LookAtTarget(const FVector& targetpos)
{
	const FVector forward = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f);
	const FVector target = FVector(targetpos.X, targetpos.Y, 0.f);
	const FVector targetDir = (targetpos - GetActorLocation()).GetSafeNormal();

	const float dtheta = FVector::DotProduct(GetActorForwardVector(), targetDir);
	double theta = FMath::Acos(dtheta);
	theta = FMath::RadiansToDegrees(theta);
	TurningValue = theta;

	const FVector CrossProduct = FVector::CrossProduct(forward, targetDir);
	//WARNINGLOG(TEXT("CrossProduct: %f"), CrossProduct.Z)
	if (CrossProduct.Z < 0)
	{
		//WARNINGLOG(TEXT("Rotation: %f"), GetActorRotation().Yaw)
		TurnLeft();
	}
	else 
	{
		//WARNINGLOG(TEXT("Rotation: %f"), GetActorRotation().Yaw)
		TurnRight();
	}
}

void ANonPlayerCharacterBase::TurnRight()
{
	FRotator newRot = GetActorRotation();
	newRot.Yaw += TurnSpeed;
	CurrentTurningValue += TurnSpeed;
	//WARNINGLOG(TEXT("Right TurnDest: %f"), newRot.Yaw)

	if ((CurrentTurningValue >= TurningValue) ||
		(NPCState == ENPCState::Death))
	{
		CurrentTurningValue = 0;
		return;
	}
	SetActorRotation(newRot);

	
	GetWorld()->GetTimerManager().SetTimer(TurningHandle, this, &ANonPlayerCharacterBase::TurnRight, 0.01f, false);
}

void ANonPlayerCharacterBase::TurnLeft()
{
	FRotator newRot = GetActorRotation();
	newRot.Yaw -= TurnSpeed;
	CurrentTurningValue += TurnSpeed;
	//WARNINGLOG(TEXT("Left TurnDest: %f"), newRot.Yaw)

	if ((CurrentTurningValue >= TurningValue) ||
		(NPCState == ENPCState::Death))
	{
		CurrentTurningValue = 0;
		return;
	}
	SetActorRotation(newRot);

	GetWorld()->GetTimerManager().SetTimer(TurningHandle, this, &ANonPlayerCharacterBase::TurnLeft, 0.01f, false);
}

void ANonPlayerCharacterBase::ClearTargetInfo()
{
	Target = nullptr;
}

void ANonPlayerCharacterBase::SetHealthBarWidget(bool bvisibility)
{
	HealthBarComponent->SetVisibility(bvisibility);
}

void ANonPlayerCharacterBase::SetHPMAX()
{
	CurrentHP = MaxHealth;
	UpdateHealthPercent(CurrentHP);
}

void ANonPlayerCharacterBase::StopMove()
{
	NPCController->StopMovement();
}

void ANonPlayerCharacterBase::MoveDown()
{
	if (!IsValid(Mover))
		return;

	Mover->MoveDown();
}





