#include "NPC/NonPlayerCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "NPC/HealthBarComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animations/CPP_NPCAnimInstance.h"
#include "Object/Mover.h"
#include "CPP_Character.h"
#include "NPC/CPP_NPCcontroller.h"

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

	DelfaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	SidStepSpeed = 170.f;
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

	NPCController = Cast<ACPP_NPCcontroller>(GetController());
	if (!IsValid(NPCController))
	{
		WARNINGLOG(TEXT("NPCController is not Valid!! Please Check 'AI controller class'"));
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

	GetWorldTimerManager().SetTimer(TimerHandle,this, &ANonPlayerCharacterBase::MoveDown,
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

void ANonPlayerCharacterBase::MoveSide(const FVector& pos)
{
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(pos);
	MoveRequest.SetAcceptanceRadius(3.f);
	NPCController->MoveTo(MoveRequest);
	NPCController->CanUpdateState(true);

	if (IsValid(Target))
	{
		bTurningLoop = true;
		LookAtTarget(Target->GetActorLocation());
	}
}

float ANonPlayerCharacterBase::PlayNPCMontage(UAnimMontage* montageToPlay)
{
	if (!IsValid(montageToPlay) || !IsValid(NPCAnimInstance))
	{
		WARNINGLOG(TEXT("is not valid montage or NPCAnimInstance!!"))
		return 0.f;
	}
		
	NPCAnimInstance->Montage_Play(montageToPlay);
	
	return montageToPlay->GetPlayLength() + DELAY1;
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

	if (bTurningLoop && IsValid(Target))
	{
		FTimerHandle timerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("LookAtTarget"), Target->GetActorLocation());
		GetWorldTimerManager().SetTimer(timerHandle, TimerDel, DELAY5, false);
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
	GetWorldTimerManager().SetTimer(TurningHandle, this, &ANonPlayerCharacterBase::TurnRight, SPEED3, false);
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
	GetWorldTimerManager().SetTimer(TurningHandle, this, &ANonPlayerCharacterBase::TurnLeft, SPEED3, false);
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

void ANonPlayerCharacterBase::SetControlOwner(ANonPlayerCharacterBase* owner)
{
	if (!IsValid(owner))
		return;

	NPCController->SetControlOwner(owner);
}

void ANonPlayerCharacterBase::MoveDown()
{
	if (!IsValid(Mover))
		return;

	Mover->MoveDown();
}