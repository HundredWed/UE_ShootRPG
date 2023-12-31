#include "CPP_Character.h"
#include "CPP_Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Grabber.h"
#include "Item/Item.h"
#include "Item/Weapon.h"
#include "Item/Gun/Rifle.h"
#include "Item/PickUpItem.h"
#include "Camera/CameraManager.h"
#include "Widget/MainPanelWidget.h"
#include "Inventory.h"
#include "Item/WeaponAbiliys/WeaponAbilityBase.h"

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
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	//jump
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	CameraManager = CreateDefaultSubobject<UCameraManager>(TEXT("Camera Manager"));

	GameInventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
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

	if (IsValid(CameraManager))
	{
		CameraManager->SetSpringArm(CameraBoom);
		CameraManager->SetCamera(FollowCamera);

		CameraManager->SetBeginCamera();
	}

	GraberComponent = FindComponentByClass<UGrabber>();
	if (IsValid(GraberComponent))
	{
		UE_LOG(LogTemp, Display, TEXT("Found GraberComponent! "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Graver component not found!!"));
	}

	if (MainPanelclass)
	{
		MainPanelWidget = CreateWidget<UMainPanelWidget>(GetWorld(), MainPanelclass);
		MainPanelWidget->AddToViewport();
	}

	GameInventory = FindComponentByClass<UInventory>();
	if (IsValid(GameInventory))
	{
		GameInventory->InventoryWidget = MainPanelWidget->GetInventoryWidget();
		HideGameInventory();
		UE_LOG(LogTemp, Display, TEXT("Found Inventory! "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory component not found!!"));
	}

	/**ignore from item trace*/
	Params.AddIgnoredActor(this);
}

void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SreachItem();

	SetMouseRate();
	CalculateCrosshairSpread(DeltaTime);
}

void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_Character::Look);
		EnhancedInputComponent->BindAction(SpeedAction, ETriggerEvent::Triggered, this, &ACPP_Character::SetSpeed);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPP_Character::Jump);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACPP_Character::SetCrouch);

 		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Canceled, this, &ACPP_Character::PickUp);
		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Triggered, this, &ACPP_Character::GrabItem);
		
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACPP_Character::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACPP_Character::Attack);

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ACPP_Character::Aiming);

		EnhancedInputComponent->BindAction(InventoryToggle, ETriggerEvent::Triggered, this, &ACPP_Character::InventoryVisibility);
	}

}

void ACPP_Character::SreachItem()
{
	if (bCanSearchObject)
	{
		ObjectSearchTrace();
		//DISPLAYLOG(TEXT("search item"));
	}
	else if (IsValid(HitResultObject) && !bCanSearchObject)
	{
		ResetHitResultState();
		//DISPLAYLOG(TEXT("reset hit result"))
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
			//UE_LOG(LogTemp, Display, TEXT("%s"), *hitresult->GetActorNameOrLabel());

			APickUpItem* item = Cast<APickUpItem>(hitresult);
			if (IsValid(item) && item->GetWidgetComponent())
			{
				item->SetWidgetVisibility(true);
			}

			if (IsValid(PrevHitResultObject))
			{
				if (PrevHitResultObject != item)
				{
					PrevHitResultObject->SetWidgetVisibility(false);
				}
			}

			PrevHitResultObject = item;
			SetHitResultObject(item);
		}
		
	}
	else
	{
		ResetHitResultState();
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
		ECollisionChannel::ECC_GameTraceChannel3,
		Sphere,
		Params);
}


void ACPP_Character::Move(const FInputActionValue& Value)
{
	const FVector MovementVector =  Value.Get<FVector>();
	CameraBoom->bEnableCameraLag = false;
	if (IsValid(GetController()))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void ACPP_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (IsValid(GetController()))
	{
		AddControllerYawInput(LookAxisVector.X * MouseRate);
		AddControllerPitchInput(LookAxisVector.Y * MouseRate);
	}
}

void ACPP_Character::SetSpeed(const FInputActionValue& Value)
{
	if (PressKey(Value))
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveRunSpeed;
		bShiftDown = true;
		//UE_LOG(LogTemp, Display, TEXT("Fest!!"));
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveDelfaultSpeed;
		bShiftDown = false;
		//UE_LOG(LogTemp, Display, TEXT("walk"));
	}
}

void ACPP_Character::GrabItem(const FInputActionValue& Value)
{
	if(!IsValid(GraberComponent))
		return;
	
	if (PressKey(Value))
	{
		GraberComponent->Grab();
		UE_LOG(LogTemp, Display, TEXT("Grab"));
	}
	else
	{
		GraberComponent->Release();
		UE_LOG(LogTemp, Display, TEXT("Release"));
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
		if (PickUpWeapon())
			return;

		//else item
		//UE_LOG(LogTemp, Display, TEXT("PickUp!!"));
		HitResultObject->TakePickUp(this);
		RemoveHitResultObject();
	}
	
}

void ACPP_Character::Equip(const FInputActionValue& Value)
{
	bool bEquipedWeapon = IsValidEquipWeapon();
	if (CanEquipState() && bEquipedWeapon)
	{
		SetStateEquipped();
	}
	else if(CanUnEquipState() && bEquipedWeapon)
	{
		SetStateUnEquipped();
	}
}

void ACPP_Character::Attack(const FInputActionValue& Value)
{
	ARifle* rifle = Cast<ARifle>(EquippedWeapon);
	if(!IsValid(rifle))
		return;

	if (CanAttackState())
	{
		if(bAiming)
		{
			PlayFireMontage(AimingFireMontage);
		}
		else
		{
			PlayFireMontage(FireMontage);
		}
		
		rifle->PullTrigger();
		ActionWeaponAbility();
	}
}

void ACPP_Character::Aiming(const FInputActionValue& Value)
{
	if (PressKey(Value) && CharacterState == ECharacterStateTypes::Equiped && ActionState == ECharacterActionState::Normal)
	{
		bAiming = true;	
		if (GetCharacterMovement()->IsCrouching())
		{
			GetCharacterMovement()->MaxWalkSpeedCrouched = MoveAimingSpeed_Crouch;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveAimingSpeed;
		}
	}
	else
	{
		bAiming = false;
		if (GetCharacterMovement()->IsCrouching())
		{
			GetCharacterMovement()->MaxWalkSpeedCrouched = MoveDelfaultSpeed_Crouch;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveDelfaultSpeed;
		}
	}
}

void ACPP_Character::SetCrouch(const FInputActionValue& Value)
{
	bool bcanCrouch = !GetCharacterMovement()->IsFalling() && PressKey(Value);
	if (bcanCrouch && !GetCharacterMovement()->IsCrouching())
	{
		Crouch();	
		CameraBoom->bEnableCameraLag = true;
	}
	else if (bcanCrouch && GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
		CameraBoom->bEnableCameraLag = true;
	}
}

void ACPP_Character::InventoryVisibility(const FInputActionValue& Value)
{
	if (PressKey(Value))
	{
		if (isVisible)
		{
			HideGameInventory();
		}
		else if(!isVisible)
		{
			ShowGameInventory();
		}
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
	if (!IsValid(weapon))
	{
		return nullptr;
	}

	return weapon;
}

bool ACPP_Character::PickUpWeapon()
{
	AWeapon* weapon = isWeapon(HitResultObject);

	if (IsValid(weapon))
	{
		if (EquippedWeapon)
		{
			if (IsInActivePrevEquippedWeapon())
			{
				EquippedWeapon->Destroy();
				SetEquippedWeapon(weapon);
				return true;
			}

			return false;
		}
		else
		{
			SetEquippedWeapon(weapon);
			return true;
		}
	}

	return false;
	
}

bool ACPP_Character::IsInActivePrevEquippedWeapon()
{
	if(IsValid(EquippedWeapon))
	{
		return !EquippedWeapon->IsActiveWaepon();
	}
	return false;
}

void ACPP_Character::ActionWeaponAbility()
{
	
	if (WeaponAbilityStorage.Find(AbilityId) == nullptr)
		return;

	WeaponAbility[AbilityId]->WeaponAbility();
}

void ACPP_Character::ResetHitResultState()
{
	APickUpItem* item = Cast<APickUpItem>(HitResultObject);

	if (IsValid(item) && item->IsValidWidget())
	{
		item->SetWidgetVisibility(false);
	}

	if (IsValid(PrevHitResultObject))
	{
		PrevHitResultObject->SetWidgetVisibility(false);
		PrevHitResultObject = nullptr;
	}
	RemoveHitResultObject();
}

void ACPP_Character::RemoveHitResultObject()
{
	HitResultObject = nullptr;
}

void ACPP_Character::SetStateEquipped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::Equiped;

	PlayEquipMontage("Equip");
	SmoothSpringArmOffset(SpringArmSocketOffsetYValue, false);

	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	playercontroller->SetHUDVisibility(true);
	UE_LOG(LogTemp, Display, TEXT("Equiped"));
}

void ACPP_Character::SetStateUnEquipped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::UnEquiped;

	PlayEquipMontage("UnEquip");
	SmoothSpringArmOffset(0, true);

	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	playercontroller->SetHUDVisibility(false);
	UE_LOG(LogTemp, Display, TEXT("UnEquiped"));
}

void ACPP_Character::SetEquippedWeapon(AWeapon* equippedWeapon)
{
	UItem* weaponRef = equippedWeapon->GetPickUpItemRef();
	if (!IsValid(weaponRef))
		return;

	EquippedWeapon = equippedWeapon;
	SetWeaponAbility((uint8)weaponRef->WeaponAbilityID);

	equippedWeapon->SetOwner(this);

	/**equip weapon after SetOwner,TakePlayerATk*/
	equippedWeapon->Equip(GetMesh(), "weapon_socket_back");
	
	/**search trace ignor*/
	Params.AddIgnoredActor(equippedWeapon);

	GameInventory->UpdateEquipmentInventory(equippedWeapon->GetPickUpItemRef());
}

AWeapon* ACPP_Character::GetEquippedWeapon()
{
	return EquippedWeapon;
}

bool ACPP_Character::CanAttackState()
{
	return CharacterState == ECharacterStateTypes::Equiped && ActionState == ECharacterActionState::Normal;
}

bool ACPP_Character::CanEquipState()
{
	return CharacterState == ECharacterStateTypes::UnEquiped 
		&& !GetCharacterMovement()->IsFalling() 
		&& ActionState == ECharacterActionState::Normal;
}

bool ACPP_Character::CanUnEquipState()
{
	return CharacterState == ECharacterStateTypes::Equiped
		&& !GetCharacterMovement()->IsFalling()
		&& ActionState == ECharacterActionState::Normal;;
}

bool ACPP_Character::IsValidEquipWeapon()
{
	return IsValid(EquippedWeapon) && EquippedWeapon->IsActiveWaepon();
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

void ACPP_Character::PlayFireMontage(UAnimMontage* montage)
{
	if(IsValid(montage))
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		if(IsValid(SkeletalMeshComponent))
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (IsValid(AnimInstance))
			{
				AnimInstance->Montage_Play(montage);
				return;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Can't montage play!"));
}


void ACPP_Character::HoldWeapon()
{
	if (IsValid(EquippedWeapon))
	{
		EquippedWeapon->AttachFunc(GetMesh(), "weapon_socket_r");
	}
}

void ACPP_Character::UnHoldWeapon()
{
	if (IsValid(EquippedWeapon))
	{
		EquippedWeapon->AttachFunc(GetMesh(), "weapon_socket_back");
	}
}

void ACPP_Character::EquippingEnd()
{
	UE_LOG(LogTemp, Display, TEXT("EquippingEnd!"));
	ActionState = ECharacterActionState::Normal;
}

void ACPP_Character::SetMouseRate()
{
	if (bAiming)
	{
		MouseRate = ClampRnage(AimingMouseRate);
	}
	else
	{
		MouseRate = ClampRnage(HipMouseRate);
	}
}

void ACPP_Character::CalculateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.f, 600.f };
	FVector2D VelocityMultiplierRange{ 0.f, 1.f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size());

	if (GetCharacterMovement()->IsFalling())
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
	}

	if (bAiming)
	{
		CrosshairSpreadMultiplier = FMath::FInterpTo(CrosshairSpreadMultiplier, 0.f, DeltaTime, 10.f);
	}
	else
	{
		CrosshairSpreadMultiplier = 0.5f + CrosshairVelocityFactor + CrosshairInAirFactor;
	}

}

float ACPP_Character::GetCrosshairSpreadMultiplier() const
{
	return CrosshairSpreadMultiplier;
}

void ACPP_Character::SetHitResultObject(APickUpItem* hitresultobject)
{
	HitResultObject = hitresultobject;
}

void ACPP_Character::HideGameInventory()
{
	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	if (IsValid(playercontroller) == false)
	{
		return;
	}
	
	playercontroller->HideCursor();

	isVisible = false;
	GameInventory->HideInventory();
}

void ACPP_Character::ShowGameInventory()
{
	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	if (IsValid(playercontroller) == false)
	{
		return;
	}

	playercontroller->ShowCursor();

	isVisible = true;
	GameInventory->ShowInventory();
}


void ACPP_Character::SetWeaponAbility(const uint8 id)
{
	AbilityId = id;

	UWeaponAbilityBase* ability = UWeaponAbilityBase::GetAbility(id);
	if (!IsValid(ability))
		return;

	WeaponAbilityStorage.Add(id);
	WeaponAbility.Add(id, ability);
}

float ACPP_Character::ClampRnage(float value)
{
	FVector2D Input(0.f, 100.f);
	FVector2D Output(0.f, 1.f);

	return  FMath::GetMappedRangeValueClamped(Input, Output, value);
}

void ACPP_Character::SmoothSpringArmOffset(float NewYoffset, bool bOrientRotationToMovement)
{
	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	CameraManager->NewValue = NewYoffset;
}

