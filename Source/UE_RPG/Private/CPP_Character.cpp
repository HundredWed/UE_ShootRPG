#include "CPP_Character.h"
#include "CPP_Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "Grabber.h"
#include "Item/Item.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "Item/Weapon.h"
#include "Item/PickUpItem.h"
#include "Camera/CameraManager.h"
#include "Widget/MainPanelWidget.h"
#include "Widget/NPC/CPP_DamageActor.h"
#include "Inventory.h"
#include "Item/Weapon/CPP_WeaponManager.h"


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
	GetCharacterMovement()->RotationRate.Yaw = DefaultMRR;
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

	WeaponManager = CreateDefaultSubobject<UCPP_WeaponManager>(TEXT("WeaponManager"));
}


void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentMana = MaxMana;
	CurrentStamina = MaxStamina;

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

	if (IsValid(MainPanelclass))
	{
		MainPanelWidget = CreateWidget<UMainPanelWidget>(GetWorld(), MainPanelclass);

		if (IsValid(MainPanelWidget))
		{
			MainPanelWidget->AddToViewport();
			MainPanelWidget->InitState(Level, CurrentHealth, MaxHealth, MaxMana, CurrentStamina);
		}
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
		EnhancedInputComponent->BindAction(DodgeToggle, ETriggerEvent::Triggered, this, &ACPP_Character::Dodge);

 		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Canceled, this, &ACPP_Character::PickUp);
		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Triggered, this, &ACPP_Character::GrabItem);
		
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACPP_Character::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACPP_Character::Attack);

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ACPP_Character::Aiming);

		EnhancedInputComponent->BindAction(InventoryToggle, ETriggerEvent::Triggered, this, &ACPP_Character::InventoryVisibility);
	}

}

float ACPP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (ActionState == ECharacterActionState::SuperAction)
		return 0.0f;

	if(ActionState != ECharacterActionState::Action)
		PlayMontage(DamagedMontage);

	DecreasePlayerHP(DamageAmount);
	return DamageAmount;
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
	if (ActionState == ECharacterActionState::SuperAction)
		return;

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

		//
		float speed = GetCharacterMovement()->Velocity.Length();
		bMoving = (Value.Get<FVector>().Length()) != 0 && (speed >= 3.f);
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
	if (!IsValid(HitResultObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("no item!!"));
		return;
	}
	else
	{
		bool equipWeapon = PickUpWeapon();
		if (!equipWeapon)
			HitResultObject->TakePickUp(this);/**inventory pick up*/

		RemoveHitResultObject();
	}
	
}

void ACPP_Character::Equip(const FInputActionValue& Value)
{
	//bool bEquipedWeapon = IsValidEquipWeapon();
	if (CanEquipState())
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, TEXT("Equipped!!"));
		SetStateEquipped();
	}
	else if(CanUnEquipState())
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("UnEquipped!!"));
		SetStateUnEquipped();
	}
}

void ACPP_Character::Attack(const FInputActionValue& Value)
{
	if(CharacterState == ECharacterStateTypes::UnEquiped || CharacterState == ECharacterStateTypes::Death)
		return;
	
	PressFireKey = PressKey(Value);
	
	if (bTrigger)
	{
		AttackWeapon();
		CharacterState = ECharacterStateTypes::Aim;
	}
}

void ACPP_Character::Aiming(const FInputActionValue& Value)
{
	if (PressKey(Value) && CharacterState > ECharacterStateTypes::Normal && ActionState == ECharacterActionState::Normal)
	{
		bAiming = true;	
		CharacterState = ECharacterStateTypes::Aim;
		SetMovementRotate(false, FocusingMRR);
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
		CharacterState = ECharacterStateTypes::Equiped;
		
		SetMovementRotate(true, DefaultMRR);
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

void ACPP_Character::Dodge(const FInputActionValue& Value)
{
	if (ActionState != ECharacterActionState::SuperAction && CurrentStamina >= 10)
	{
		bMoving = false;
		LookAt();
		PlayMontage(DodgeMontage);
		CurrentStamina -= 10;
		ActionState = ECharacterActionState::SuperAction;
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

void ACPP_Character::KnockBack(const FVector& velocity)
{
	if (ActionState == ECharacterActionState::SuperAction)
		return;

	ActionState = ECharacterActionState::SuperAction;
	LaunchCharacter(velocity, true, false);

	FTimerHandle th;
	GetWorldTimerManager().SetTimer(th, this, &ACPP_Character::SuperActionEnd, DELAY3, false);
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

//AWeapon* ACPP_Character::isWeapon(AActor* hitobject) const
//{
//	AWeapon* weapon = Cast<AWeapon>(hitobject);
//	if (!IsValid(weapon))
//	{
//		return nullptr;
//	}
//
//	return weapon;
//}

bool ACPP_Character::PickUpWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		SetEquipWeapon(HitResultObject->GetPickUpItemRef());
		return true;
	}
	
	return false;
}

void ACPP_Character::AttackWeapon()
{
	if (CanAttackState())
	{
		SetMovementRotate(false, FocusingMRR);

		if (bAiming)
		{
			PlayMontage(AimingFireMontage);
		}
		else
		{
			PlayMontage(FireMontage);
		}

		CurrentWeapon->Attack();

		bTrigger = false;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Character::CanTrigger, TriggerRate, false);
	}

	/*if (PressFireKey or when a skill or other trigger condition is true)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Character::FireWeapon, FireRate, false);
	}*/
}

void ACPP_Character::CanTrigger()
{
	bTrigger = true;
}

void ACPP_Character::SetMovementRotate(bool bORT, float rotationRate)
{
	GetCharacterMovement()->bOrientRotationToMovement = bORT;
	GetCharacterMovement()->RotationRate.Yaw = rotationRate;
}

void ACPP_Character::LookAt()
{
	FRotator currentRot = GetActorRotation();

	FRotator AimRotation = GetBaseAimRotation();
	FRotator newRot;
	if (GetCharacterMovement()->Velocity.Length() == 0 || CharacterState != ECharacterStateTypes::Aim)
	{
		return;
	}
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(GetCharacterMovement()->Velocity);
	newRot.Yaw = currentRot.Yaw + UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;;
	SetActorRotation(newRot);
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
	HitResultObject = nullptr;
}

void ACPP_Character::RemoveHitResultObject()
{
	if (!IsValid(HitResultObject))
		return;

	HitResultObject->Destroy();
	HitResultObject = nullptr;
}

void ACPP_Character::SetStateEquipped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::Equiped;

	PlayEquipMontage("Equip");
	SmoothSpringArmOffset(SpringArmSocketOffsetYValue);

	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	playercontroller->SetHUDVisibility(true);
	UE_LOG(LogTemp, Display, TEXT("Equiped"));
}

void ACPP_Character::SetStateUnEquipped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::UnEquiped;

	PlayEquipMontage("UnEquip");
	SmoothSpringArmOffset(0);
	SetMovementRotate(true, DefaultMRR);

	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	playercontroller->SetHUDVisibility(false);
	UE_LOG(LogTemp, Display, TEXT("UnEquiped"));
}

void ACPP_Character::SetEquipWeapon(UItem* item)
{
	WeaponManager->EquipWeapon(item->ItemInfoID, item->WeaponActor);
	CurrentWeapon = WeaponManager->GetCurrntWeapon();
	CurrentWeapon->SetOwner(this);

	GameInventory->UpdateEquipmentInventory(item);
	CharacterState = ECharacterStateTypes::UnEquiped;
}

void ACPP_Character::TakeOffWeapon()
{
	WeaponManager->TakeOffWeapon();
	SetStateUnEquipped();
}

bool ACPP_Character::CanAttackState()
{
	return (CharacterState == ECharacterStateTypes::Equiped || CharacterState == ECharacterStateTypes::Aim)
		&& ActionState == ECharacterActionState::Normal
		&& PressFireKey;
}

bool ACPP_Character::CanEquipState()
{
	return CharacterState == ECharacterStateTypes::UnEquiped 
		&& !GetCharacterMovement()->IsFalling() 
		&& ActionState == ECharacterActionState::Normal;
}

bool ACPP_Character::CanUnEquipState()
{
	return (CharacterState == ECharacterStateTypes::Equiped)
		&& !GetCharacterMovement()->IsFalling()
		&& ActionState == ECharacterActionState::Normal;
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

void ACPP_Character::PlayMontage(UAnimMontage* montage)
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
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Equip(GetMesh(), "weapon_socket_r");
	}
}

void ACPP_Character::UnHoldWeapon()
{
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->Equip(GetMesh(), "weapon_socket_back");
	}
}

void ACPP_Character::EquippingEnd()
{
	UE_LOG(LogTemp, Display, TEXT("EquippingEnd!"));
	ActionState = ECharacterActionState::Normal;
}

void ACPP_Character::SuperActionEnd()
{
	ActionState = ECharacterActionState::Normal;
	//DISPLAYLOG(TEXT("bOrientRotationToMovement is true"))
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

void ACPP_Character::DecreasePlayerHP(const float value)
{
	CurrentHealth -= value;
	CurrentHealth = CurrentHealth < 0 ? 0 : CurrentHealth;

	MainPanelWidget->UpdateHealthBarPercent(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0)
		CharacterState = ECharacterStateTypes::Death;
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

void ACPP_Character::SetFireRate(float rate)
{
	FireRate = rate;
}

void ACPP_Character::SpawnWeaponBase()
{
	//FString path = TEXT("/Script/Engine.DataTable'/Game/ShootGame/Data/DT_ItemTable.DT_ItemTable'");
	//UDataTable* dataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *path));
}

void ACPP_Character::StoreDamageUI()
{
	//Á¦°Å
	UWorld* world = GetWorld();
	const int32 amount = 50;

	if (IsValid(world) && IsValid(DamageUIActorClass))
	{
		for (int32 i = 0; i < amount; i++)
		{
			ACPP_DamageActor* damageActor = world->SpawnActor<ACPP_DamageActor>(DamageUIActorClass);
			DamageUIActors.Push(damageActor);
		}
	}
	else
	{
		WARNINGLOG(TEXT("is not valid DamageUIActorClass!!"))
	}
}

ACPP_DamageActor* ACPP_Character::GetDamageActor()
{
	NextUI = NextUI > (DamageUIActors.Num() - 1) ? 0 : NextUI;
	ACPP_DamageActor* nextUI = DamageUIActors[NextUI];
	NextUI++;

	return nextUI;
}

int32 ACPP_Character::GetDamageUIArrayLength()
{
	return DamageUIActors.Num();
}

float ACPP_Character::ClampRnage(float value)
{
	FVector2D Input(0.f, 100.f);
	FVector2D Output(0.f, 1.f);

	return  FMath::GetMappedRangeValueClamped(Input, Output, value);
}

void ACPP_Character::SmoothSpringArmOffset(float NewYoffset)
{
	//GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	CameraManager->NewValue = NewYoffset;
}

