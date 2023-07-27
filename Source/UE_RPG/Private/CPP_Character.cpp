#include "CPP_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CharacterSpringArm.h"
#include "Grabber.h"
#include "Item/Weapon.h"
#include "Item/Gun/ShootGun.h"


ACPP_Character::ACPP_Character()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//CharacterMovement Settting Base 
	GetCharacterMovement()->GravityScale = 1.75f; 
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	//jump
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;


	SpringArm = CreateDefaultSubobject<UCharacterSpringArm>(TEXT("CameraBoom"));
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
	if (IsValid(GraberComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Found GraberComponent! "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Graver component not found!!"));
	}

	if (IsValid(FollowCamera))
	{
		CameraDefaultFOV = FollowCamera->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	Params.AddIgnoredActor(this);
}

void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ObjectSearchTrace();
	SmoothCameraFOV(DeltaTime);
}

void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_Character::Look);
		EnhancedInputComponent->BindAction(SpeedAction, ETriggerEvent::Triggered, this, &ACPP_Character::SetSpeed);

		EnhancedInputComponent->BindAction(JumppAction, ETriggerEvent::Triggered, this, &ACPP_Character::Jump);

 		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Canceled, this, &ACPP_Character::PickUp);
		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Triggered, this, &ACPP_Character::GrabItem);
		
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACPP_Character::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACPP_Character::Attack);

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ACPP_Character::Aiming);
	}

}

void ACPP_Character::ObjectSearchTrace()
{
	FHitResult HitResult;
	bool OnHit = SetShpereTrace(HitResult);

	if (OnHit)
	{
		AActor* hitresult = HitResult.GetActor();
		if (IsValid(hitresult))
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *hitresult->GetActorNameOrLabel());
			SetHitResultObject(hitresult);
		}
		
	}
	else
	{
		RemoveHitResultObject();
	}
}

bool ACPP_Character::SetShpereTrace(FHitResult& HitResult)
{
	FVector Location;
	FRotator Rotation;
	GetViewPointVector(Location, Rotation);
	FVector End = Location + Rotation.Vector() * ShowItemDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(ShowItemRadius);
	

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
	if (IsValid(Controller))
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
	if (IsValid(Controller))
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
	if (IsValid(HitResultObject) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("no item!!"));
		return;
	}
	else
	{
		AWeapon* weapon = isWeapon(HitResultObject);
		bool bweapon = IsValid(weapon);
		if (weapon)
		{
			weapon->Equip(GetMesh(),"weapon_socket_back");
			weapon->SetOwner(this);
			EquipedWeapon = weapon;
			Params.AddIgnoredActor(weapon);
			weapon->SetIsGrabbable(false);
			return;
		}

		//else item
		UE_LOG(LogTemp, Warning, TEXT("PickUp!!"));
	}
	
}

void ACPP_Character::Equip(const FInputActionValue& Value)
{
	bool bEquipedWeapon = IsValid(EquipedWeapon);
	if (CharacterState == ECharacterStateTypes::UnEquiped && bEquipedWeapon)
	{
		SetStateEquiped();
	}
	else if(CharacterState == ECharacterStateTypes::Equiped)
	{
		SetStateUnEquiped();
	}
}

void ACPP_Character::Attack(const FInputActionValue& Value)
{
	AShootGun* shootgun = Cast<AShootGun>(EquipedWeapon);
	bool bshootgun = IsValid(shootgun);

	if (CanAttackState() && bshootgun)
	{
		PlayFireMontage();
		shootgun->PullTrigger();
	}
}

void ACPP_Character::Aiming(const FInputActionValue& Value)
{
	if (PressKey(Value) && CharacterState == ECharacterStateTypes::Equiped)
	{
		bAiming = true;
	}
	else
	{
		bAiming = false;
	}
}


void ACPP_Character::GetViewPointVector(FVector& Location, FRotator& Rotation)
{
	AController* MyController = GetController();
	if (IsValid(MyController) == false)
	{
		return;
	}
	MyController->GetPlayerViewPoint(Location, Rotation);
}


bool ACPP_Character::PressKey(const FInputActionValue& Value)
{
	return Value.Get<bool>();
}

AWeapon* ACPP_Character::isWeapon(AActor* hitobject) const
{
	AWeapon* weapon = Cast<AWeapon>(hitobject);
	if (weapon == nullptr)
	{
		return nullptr;
	}

	return weapon;
}

void ACPP_Character::SetStateEquiped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::Equiped;

	PlayEquipMontage("Equip");
	SmoothSpringArmOffset(SpringArmSocketOffsetYValue, false);
	UE_LOG(LogTemp, Warning, TEXT("Equiped"));
}

void ACPP_Character::SetStateUnEquiped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::UnEquiped;

	PlayEquipMontage("UnEquip");
	SmoothSpringArmOffset(0, true);
	UE_LOG(LogTemp, Warning, TEXT("UnEquiped"));
}

bool ACPP_Character::CanAttackState()
{
	return CharacterState == ECharacterStateTypes::Equiped && ActionState == ECharacterActionState::Normal;
}

void ACPP_Character::PlayEquipMontage(FName NotifyName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	bool bEquipMontage = IsValid(EquipMontage);
	if (AnimInstance && bEquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(NotifyName, EquipMontage);
	}
}

void ACPP_Character::PlayFireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	bool bFireMontage = IsValid(FireMontage);
	if (AnimInstance && bFireMontage)
	{
		AnimInstance->Montage_Play(FireMontage);
	}
}

void ACPP_Character::HoldWeapon()
{
	if (IsValid(EquipedWeapon))
	{
		EquipedWeapon->Equip(GetMesh(), "weapon_socket_r");
	}
}

void ACPP_Character::UnHoldWeapon()
{
	if (IsValid(EquipedWeapon))
	{
		EquipedWeapon->Equip(GetMesh(), "weapon_socket_back");
	}
}

void ACPP_Character::EquippingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("EquippingEnd!"));
	ActionState = ECharacterActionState::Normal;
}

void ACPP_Character::SetHitResultObject(AActor* hitresultobject)
{
	if (HitResultObject != nullptr)
	{
		return;
	}

	HitResultObject = hitresultobject;
}

void ACPP_Character::RemoveHitResultObject()
{
	HitResultObject = nullptr;
}

void ACPP_Character::SmoothSpringArmOffset(float NewYoffset, bool bOrientRotationToMovement)
{
	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	SpringArm->NewValue = NewYoffset;
}

void ACPP_Character::SmoothCameraFOV(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	FollowCamera->SetFieldOfView(CameraCurrentFOV);
}
