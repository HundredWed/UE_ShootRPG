#include "CPP_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grabber.h"


ACPP_Character::ACPP_Character()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/*GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;*/


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm);
	FollowCamera->bUsePawnControlRotation = false;
}


void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GraberComponent = FindComponentByClass<UGrabber>();
	if (GraberComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found GraberComponent! "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Graver components found!!"));
	}
}

void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ObjectSearchTrace();
}

void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_Character::Look);
		EnhancedInputComponent->BindAction(SpeedAction, ETriggerEvent::Triggered, this, &ACPP_Character::SetSpeed);

 		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Canceled, this, &ACPP_Character::PickUp);
		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Triggered, this, &ACPP_Character::GrabItem);
		
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACPP_Character::Equip);
	}

}

void ACPP_Character::ObjectSearchTrace()
{
	FHitResult HitResult;
	bool OnHit = SetShpereTrace(HitResult);

	if (OnHit)
	{
		AActor* actor = HitResult.GetActor();
		if (actor == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("null!!"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *actor->GetActorNameOrLabel());
		}
	}
}

bool ACPP_Character::SetShpereTrace(FHitResult& HitResult)
{
	FVector Location;
	FRotator Rotation;
	GetViewPointVector(Location, Rotation);
	FVector End = Location + Rotation.Vector() * ShowItemDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(ShowItemRadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	return GetWorld()->SweepSingleByChannel(
		HitResult,
		Location,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere,
		Params);
}


void ACPP_Character::Move(const FInputActionValue& Value)
{
	const FVector MovementVector =  Value.Get<FVector>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);

		//UE_LOG(LogTemp, Warning, TEXT("Move!!"));
	}
}

void ACPP_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACPP_Character::SetSpeed(const FInputActionValue& Value)
{
	if (PressKey(Value))
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		UE_LOG(LogTemp, Warning, TEXT("Fest!!"));
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		UE_LOG(LogTemp, Warning, TEXT("walk"));
	}
}

void ACPP_Character::GrabItem(const FInputActionValue& Value)
{	
	if (PressKey(Value))
	{
		GraberComponent->Grab();
		UE_LOG(LogTemp, Warning, TEXT("Grab"));
	}
	else
	{
		GraberComponent->Release();
		UE_LOG(LogTemp, Warning, TEXT("Release"));
	}
}

void ACPP_Character::PickUp(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("PickUp!!"));
}

void ACPP_Character::Equip(const FInputActionValue& Value)
{
	if (CharacterState == ECharacterStateTypes::UnEquiped)
	{
		CharacterState = ECharacterStateTypes::Equiped;
		UE_LOG(LogTemp, Warning, TEXT("Equiped"));
	}
	else if(CharacterState == ECharacterStateTypes::Equiped)
	{
		CharacterState = ECharacterStateTypes::UnEquiped;
		UE_LOG(LogTemp, Warning, TEXT("UnEquiped"));
	}
}

bool ACPP_Character::PressKey(const FInputActionValue& Value)
{
	return Value.Get<bool>();
}


void ACPP_Character::GetViewPointVector(FVector& Location, FRotator& Rotation)
{
	AController* MyController = GetController();
	if (MyController == nullptr)
	{
		return;
	}
	MyController->GetPlayerViewPoint(Location, Rotation);
}