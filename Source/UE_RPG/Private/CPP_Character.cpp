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
#include "DrawDebugHelpers.h"

#include "Grabber.h"
#include "Item/Weapon/CPP_WeaponBase.h"
#include "Item/PickUpItem.h"
#include "Camera/CameraManager.h"
#include "Widget/NPC/CPP_DamageActor.h"
#include "Inventory.h"
#include "Item/Weapon/CPP_WeaponManager.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Component/CPP_StatComponent.h"
#include "Animations/CPP_AnimInstance.h"


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

	StatComponent = CreateDefaultSubobject<UCPP_StatComponent>(TEXT("StatComponent"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	CameraManager = CreateDefaultSubobject<UCameraManager>(TEXT("Camera Manager"));

	GameInventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
	GameInventory->OnItemRemoved.BindUObject(this, &ACPP_Character::OnRemoveItemEvent);

	WeaponManager = CreateDefaultSubobject<UCPP_WeaponManager>(TEXT("WeaponManager"));
}


void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();

	//위젯
	HideGameInventory();

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

	UGameInstance* GI = GetGameInstance();
	if (!IsValid(GI))
	{
		return;
	}
	QuestSubsystem = GI->GetSubsystem<UCPP_QuestSubsystem>();
	QuestSubsystem->OnQuestClear.AddUObject(this, &ACPP_Character::OnQuestClearEvent);


	/**ignore from item trace*/
	Params.AddIgnoredActor(this);

	StartSearTrace();
}

void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetMouseRate();
	CalculateCrosshairSpread(DeltaTime);
}

void ACPP_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TMap<EEquipmentType, FEquipmentSlot> slots = GameInventory->GetEquipmentSlots();
	if (slots.IsEmpty())return;

	if (FEquipmentSlot* slotData = slots.Find(EEquipmentType::Weapon))
	{
		WeaponManager->EquipWeapon(slotData->EquipmentID);
		CharacterState = ECharacterStateTypes::UnEquipped;
	}	
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

 		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Canceled, this, &ACPP_Character::Interact);
		EnhancedInputComponent->BindAction(GrabAndPickupAction, ETriggerEvent::Triggered, this, &ACPP_Character::GrabItem);
		
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACPP_Character::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACPP_Character::Attack);

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ACPP_Character::Aiming);

		EnhancedInputComponent->BindAction(InventoryToggle, ETriggerEvent::Triggered, this, &ACPP_Character::InventoryVisibility);
		EnhancedInputComponent->BindAction(QuestListToggle, ETriggerEvent::Triggered, this, &ACPP_Character::QuestListVisibility);
	}

}

float ACPP_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (ActionState == ECharacterActionState::SuperAction)
		return 0.0f;

	if(ActionState != ECharacterActionState::Action)
		PlayMontage(DamagedMontage);

	if (!StatComponent->DecreaseHP(DamageAmount))
	{
		CharacterState = ECharacterStateTypes::Death;

		if (ACPP_Controller* PC = Cast<ACPP_Controller>(GetController()))
		{
			PC->HandlePlayerDeath();
		}
	}
	return DamageAmount;
}

void ACPP_Character::ObjectSearchTrace()
{
	//DISPLAYLOG(TEXT(""))

	FHitResult HitResult;

	bool OnHit = SetSphereTrace(HitResult);

	if (OnHit)
	{
		AActor* hitresult = HitResult.GetActor();
		if (IsValid(hitresult))
		{
			LookAtObject(hitresult);

			if (IsValid(PrevHitResultObject))
			{
				if (PrevHitResultObject != hitresult)
				{
					EndLookAtObject(PrevHitResultObject);
				}
			}

			PrevHitResultObject = hitresult;
			SetHitResultObject(hitresult);
		}
	}
	else
	{
		ResetHitResultState();
	}
	
}

bool ACPP_Character::SetSphereTrace(FHitResult& HitResult)
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
	bMoveKeyDown = PressKey(Value);

	if (IsValid(GetController()))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		FVector DesiredDirection = (ForwardDirection * MovementVector.Y) + (RightVector * MovementVector.X);
		if (DesiredDirection.SizeSquared() > 0.0f)
		{
			InputDir = UKismetMathLibrary::MakeRotFromX(DesiredDirection);
		}


		if (ActionState == ECharacterActionState::SuperAction)
			return;

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
		GetCharacterMovement()->MaxWalkSpeed = MoveDefaultSpeed;
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

void ACPP_Character::Interact(const FInputActionValue& Value)
{
	if (!IsValid(HitResultObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("no item!!"));
		return;
	}
	
	if (ICPP_InteractInterface* interactObj = Cast<ICPP_InteractInterface>(HitResultObject))
	{
		interactObj->RequestInteract(this);
	}

	RemoveHitResultObject();	
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
	if(!IsUnderArm())
		return;
	
	PressFireKey = PressKey(Value);
	
	if (bTrigger)
	{
		AttackWeapon();
	}
}

void ACPP_Character::Aiming(const FInputActionValue& Value)
{
	if (PressKey(Value) && IsUnderArm() && ActionState == ECharacterActionState::Normal)
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
		
		if (CharacterState == ECharacterStateTypes::Aim)
		{
			CharacterState = ECharacterStateTypes::Equipped;
		}
		
		SetMovementRotate(true, DefaultMRR);
		if (GetCharacterMovement()->IsCrouching())
		{
			GetCharacterMovement()->MaxWalkSpeedCrouched = MoveDefaultSpeed_Crouch;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MoveDefaultSpeed;
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
	if (ActionState != ECharacterActionState::SuperAction && StatComponent->DecreaseDodge())
	{
		bMoving = false;//블프 애니메이션
		LookAt();
		PlayMontage(DodgeMontage);
		ActionState = ECharacterActionState::SuperAction;
	}
}

void ACPP_Character::InventoryVisibility(const FInputActionValue& Value)
{
	if (PressKey(Value) && IsQuestListVisible == false)
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

void ACPP_Character::QuestListVisibility(const FInputActionValue& Value)
{
	if (ACPP_Controller* PC = Cast<ACPP_Controller>(GetController()))
	{
		IsQuestListVisible = PC->ToggleQuestWindow();
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

bool ACPP_Character::PickUpWeapon(const FName& itemID)
{
	if (WeaponManager->GetCurrentWeapon() == nullptr)
	{
		return SetEquipWeapon(itemID);
	}
	else
	{
		const int32 storedAmount = AddInventory(itemID);
		return storedAmount > 0;
	}
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

		const float triggerRate = WeaponManager->GetCurrentWeapon()->Attack();

		bTrigger = false;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPP_Character::CanTrigger, triggerRate, false);
	}
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
	FRotator newRot = FRotator::ZeroRotator;

	if (!bMoveKeyDown)
	{
		/**default dir is Back*/
		const FRotator backBase = CameraBoom->GetTargetRotation();
		const FRotator newBase = FRotator(0.f, backBase.Yaw, 0.f);
		const FVector backVector = FRotationMatrix(newBase).GetUnitAxis(EAxis::X) * -1.f;

		const FVector player = GetActorForwardVector();
		
		const float dtheta = FVector::DotProduct(player, backVector);
		float theta = FMath::Acos(dtheta);
		theta = FMath::RadiansToDegrees(theta);

		const FVector crossProduct = FVector::CrossProduct(player, backVector);
		if (crossProduct.Z < 0)
		{
			newRot.Yaw = currentRot.Yaw - theta;
		}
		else
		{
			newRot.Yaw = currentRot.Yaw + theta;
		}
		SetActorRotation(newRot);
		return;
	}
	else if (CharacterState != ECharacterStateTypes::Aim)
	{
		return;
	}

	newRot.Yaw = currentRot.Yaw + UKismetMathLibrary::NormalizedDeltaRotator(InputDir, AimRotation).Yaw;
	SetActorRotation(newRot);
}

void ACPP_Character::ResetHitResultState()
{
	APickUpItem* item = Cast<APickUpItem>(HitResultObject);

	if (IsValid(HitResultObject))
	{
		EndLookAtObject(HitResultObject);
	}

	if (IsValid(PrevHitResultObject))
	{
		EndLookAtObject(PrevHitResultObject);
	}

	HitResultObject = nullptr;
	PrevHitResultObject = nullptr;
}

void ACPP_Character::RemoveHitResultObject()
{
	if (!IsValid(HitResultObject))
		return;

	HitResultObject = nullptr;
}

void ACPP_Character::SetStateEquipped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::Equipped;

	PlayEquipMontage("Equip");
	SmoothSpringArmOffset(SpringArmSocketOffsetYValue);

	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	playercontroller->SetHUDVisibility(true);
	UE_LOG(LogTemp, Display, TEXT("Equipped"));
}

void ACPP_Character::SetStateUnEquipped()
{
	ActionState = ECharacterActionState::Action;
	CharacterState = ECharacterStateTypes::UnEquipped;

	PlayEquipMontage("UnEquip");
	SmoothSpringArmOffset(0);
	SetMovementRotate(true, DefaultMRR);

	ACPP_Controller* playercontroller = Cast<ACPP_Controller>(GetController());
	playercontroller->SetHUDVisibility(false);
	UE_LOG(LogTemp, Display, TEXT("UnEquipped"));
}

bool ACPP_Character::SetEquipWeapon(const FName& itemID)
{
	if (WeaponManager->EquipWeapon(itemID))
	{
		GameInventory->UpdateEquipmentInventory(itemID);
		CharacterState = ECharacterStateTypes::UnEquipped;
		
		return true;
	}

	return false;
}

void ACPP_Character::TakeOffWeapon()
{
	WeaponManager->TakeOffWeapon();
	CharacterState = ECharacterStateTypes::Normal;
}

bool ACPP_Character::CanAttackState()
{
	return (CharacterState == ECharacterStateTypes::Equipped || CharacterState == ECharacterStateTypes::Aim)
		&& ActionState == ECharacterActionState::Normal
		&& PressFireKey;
}

bool ACPP_Character::CanEquipState()
{
	return CharacterState == ECharacterStateTypes::UnEquipped 
		&& !GetCharacterMovement()->IsFalling() 
		&& ActionState == ECharacterActionState::Normal;
}

bool ACPP_Character::CanUnEquipState()
{
	return (CharacterState == ECharacterStateTypes::Equipped)
		&& !GetCharacterMovement()->IsFalling()
		&& ActionState == ECharacterActionState::Normal;
}

bool ACPP_Character::IsUnderArm()
{
	return (CharacterState == ECharacterStateTypes::Equipped) || (CharacterState == ECharacterStateTypes::Aim);
}

void ACPP_Character::ResetRootOffset()
{
	Cast<UCPP_AnimInstance>(GetMesh()->GetAnimInstance())->ResetCurrentRotate();
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
	if (IsValid(WeaponManager->GetCurrentWeapon()))
	{
		WeaponManager->GetCurrentWeapon()->Equip(GetMesh(), "weapon_socket_r");
	}
}

void ACPP_Character::UnHoldWeapon()
{
	if (IsValid(WeaponManager->GetCurrentWeapon()))
	{
		WeaponManager->GetCurrentWeapon()->Equip(GetMesh(), "weapon_socket_back");
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
		MouseRate = ClampRange(AimingMouseRate);
	}
	else
	{
		MouseRate = ClampRange(HipMouseRate);
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

void ACPP_Character::SetHitResultObject(AActor* hitresultobject)
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

void ACPP_Character::InitInventory(ACPP_Controller* PC)
{
	GameInventory->InitInventoryInfo(PC);
}

void ACPP_Character::OnQuestClearEvent(const FQuest& quest)
{
	//퀘스트 보상
	GameInventory->ExchangeQuestItems(quest.RewardItemID, quest.RewardItemAmount, quest.NeedContentID, quest.NeedCount);
	StatComponent->UpdateEXP(quest.EXP);
}

void ACPP_Character::OnRestore(ERestoreTypes restoreTypes, const float amount)
{
	switch (restoreTypes)
	{
	case ERestoreTypes::Type_None:
		break;
	case ERestoreTypes::Health:
		StatComponent->IncreaseHP(amount);
		break;
	case ERestoreTypes::Mana:
		StatComponent->IncreaseMP(amount);
		break;
	case ERestoreTypes::Stamina:
		break;
	default:
		break;
	}
}

void ACPP_Character::StoreDamageUI()
{
	//제거
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

int32 ACPP_Character::AddInventory(const FName& itemID, const int32 amount)
{
	//저장 성공한 갯수
	const int32 storedAmount = GameInventory->AddItem(itemID, amount);

	if (storedAmount == 0)
	{
		return 0;
	}

	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* questSystem = GI->GetSubsystem<UCPP_QuestSubsystem>();

		if (IsValid(questSystem))
		{
			//아이템이 들어온 갯수만큼만 퀘스트 체크
			questSystem->CheckQuestContent(itemID, storedAmount);
		}
	}

	return storedAmount;
}

void ACPP_Character::SetDialogue(const FName& id, const FTransform& transform)
{
	if (ACPP_Controller* PC = Cast<ACPP_Controller>(GetController()))
	{
		PC->SetNPCInteract(id);

		if (IsValid(HitResultObject))
		{
			CameraManager->StartDialogueCamera(HitResultObject, transform);
		}		
	}
}

float ACPP_Character::ClampRange(float value)
{
	FVector2D Input(0.f, 100.f);
	FVector2D Output(0.f, 1.f);

	return  FMath::GetMappedRangeValueClamped(Input, Output, value);
}

void ACPP_Character::LookAtObject(AActor* obj)
{
	if (ICPP_InteractInterface* interactObj = Cast<ICPP_InteractInterface>(obj))
	{
		interactObj->OnBeginLookAt();
		if (interactObj->GetType() == ECharacterTypes::NPC_Person)
		{
			OnLookAtTalker.Broadcast();
		}
	}
}

void ACPP_Character::EndLookAtObject(AActor* obj)
{
	if (ICPP_InteractInterface* interactObj = Cast<ICPP_InteractInterface>(obj))
	{
		interactObj->OnEndLookAt();
		if (interactObj->GetType() == ECharacterTypes::NPC_Person)
		{
			OnEndLookAtTalker.Broadcast();
		}
	}
}

void ACPP_Character::StartSearTrace()
{
	GetWorld()->GetTimerManager().SetTimer(
		InteractTimerHandle,
		this,
		&ACPP_Character::ObjectSearchTrace,
		0.15f,
		true
	);
}

void ACPP_Character::OnRemoveItemEvent(const FName& itemId, const int32 amount)
{
	if (IsValid(QuestSubsystem))
	{
		QuestSubsystem->CheckQuestContent(itemId,amount);
	}	
}

void ACPP_Character::SmoothSpringArmOffset(float NewYoffset)
{
	//GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
	CameraManager->NewValue = NewYoffset;
}

void ACPP_Character::EndDialogueCamera()
{
	CameraManager->EndDialogueCamera();
}

