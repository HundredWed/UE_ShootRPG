#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "UE_RPG/UtilityMecro.h"
#include "Interface/CPP_StatInterface.h"
#include "CPP_Character.generated.h"

class ANonPlayerCharacterBase;
class ACPP_DamageActor;
class UCameraManager;
class UCameraComponent;
class USpringArmComponent;
class UCPP_WeaponManager;
class UInputComponent;
class UInputAction;
class UAnimMontage;
class UMainPanelWidget;
class UGrabber;
class UInventory;
class UCPP_QuestSubsystem;
class UCPP_StatComponent;
class ACPP_Controller;

struct FQuest;

UCLASS()
class UE_RPG_API ACPP_Character : public ACharacter, public ICPP_StatInterface
{
	GENERATED_BODY()

public:
	ACPP_Character();
	
	/**Component*/
	UPROPERTY(VisibleAnywhere, Category = Compoenents)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = Compoenents)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Compoenents)
	UCPP_WeaponManager* WeaponManager;

	UPROPERTY(EditAnywhere, Category = Compoenents)
	UCPP_StatComponent* StatComponent;

	/**Input*/
	/*UPROPERTY(EditAnywhere, Category = Input)
		class UInputMappingContext* DefaultMappingContext;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)	
	UInputAction* SpeedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* GrabAndPickupAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DodgeToggle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AimingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InventoryToggle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuestListToggle;

	

	/**Montage*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AimingFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DamagedMontage;

	/**widget*/
	UPROPERTY(EditAnywhere, Category = "Player Widget")
	TSubclassOf<UMainPanelWidget> MainPanelclass;

	UPROPERTY(EditAnywhere, Category = "Player Widget")
	TSubclassOf<ACPP_DamageActor> DamageUIActorClass;

	/**spring arm*/
	UPROPERTY(EditAnywhere, Category = "EditValue")
	float SpringArmSocketOffsetYValue = 100.f;

	/**Item search issue overlap counting*/
	//uint8 OverlapCount = 0;

	/**key down info*/
	bool bShiftDown = false;/*for inventory splite*/
	bool bMoveKeyDown = false;/*for dodge dir info*/
	FRotator InputDir = FRotator::ZeroRotator;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void ObjectSearchTrace();
	bool SetSphereTrace(FHitResult& HitResult);

	/**Input*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SetSpeed(const FInputActionValue& Value);
	void GrabItem(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Aiming(const FInputActionValue& Value);
	void SetCrouch(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void InventoryVisibility(const FInputActionValue& Value);
	void QuestListVisibility(const FInputActionValue& Value);
	void KnockBack(const FVector& velocity);

	/**trace*/
	void GetViewPointVector(FVector& Location, FRotator& Rotation);

	/**camera*/
	void SmoothSpringArmOffset(float NewYoffset);

	bool PressKey(const FInputActionValue& Value);
	//class AWeapon* isWeapon(AActor* hitobject) const;
	bool PickUpWeapon(const FName& itemID);
	void AttackWeapon();
	void CanTrigger();
	void SetMovementRotate(bool bORT, float rotationRate);
	void LookAt();
	

	void ResetHitResultState();
	void RemoveHitResultObject();

	/**Character Setting*/
    void SetStateEquipped();
	void SetStateUnEquipped();
	bool SetEquipWeapon(const FName& itemID);
	void TakeOffWeapon();
	bool CanAttackState();
	bool CanEquipState();
	bool CanUnEquipState();
	bool IsUnderArm();

	virtual void OnRestore(ERestoreTypes restoreTypes, const float amount) override;


	/**Montage*/
	void PlayEquipMontage(FName NotifyName);
	void PlayMontage(UAnimMontage* montage);

	UFUNCTION(BlueprintCallable)
	void HoldWeapon();

	UFUNCTION(BlueprintCallable)
	void UnHoldWeapon();

	UFUNCTION(BlueprintCallable)
	void EquippingEnd();

	UFUNCTION(BlueprintCallable)
	void SuperActionEnd();

	/**mouse*/
	void SetMouseRate();
	void CalculateCrosshairSpread(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE ECharacterStateTypes GetCharacterState() const { return CharacterState; }
	FORCEINLINE ECharacterActionState GetActionState() const { return ActionState; }
	FORCEINLINE void SetHitResultObject(AActor* hitresultobject);
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	//FORCEINLINE void SetCanSearchObject(bool cansearch) { bCanSearchObject = cansearch; }
		FORCEINLINE bool GetPlayerMoveState() { return bMoving; }
	
	/**inventory*/
	UFUNCTION()
	void HideGameInventory();

	void ShowGameInventory();

	void InitInventory(ACPP_Controller* PC);

	const float GetPlayerWeightInfo() { return InventoryMaxWeight; }
	void OnQuestClearEvent(const FQuest& quest);

	FORCEINLINE class UInventory* GetInventory() { return GameInventory; }
	FORCEINLINE const int16 GetInventorySize() { return InventoryAmountOfSlot; }
	FORCEINLINE const int16 GetInventoryRowSize() { return InventoryRowSize; }


	/**damage ui*/
	void StoreDamageUI();
	class ACPP_DamageActor* GetDamageActor();
	int32 GetDamageUIArrayLength();


	/**interact*/
	int32 AddInventory(const FName& itemID, const int32 amount = 1);
	void SetDialogue(const FName& id);

private:

	float ClampRange(float value);
	void LookAtObject(AActor* obj);
	void EndLookAtObject(AActor* obj);
	void StartSearTrace();

	UFUNCTION()
	void OnRemoveItemEvent(const FName& itemId, const int32 amount);

private:

	/**character states*/
	ECharacterStateTypes CharacterState = ECharacterStateTypes::Normal;
	ECharacterActionState ActionState = ECharacterActionState::Normal;

	/**character states - weapon*/
	bool PressFireKey = false;
	bool bTrigger = true;
	bool bMoving = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
	float DefaultMRR = 500.f;//MovementRotationRate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
	float FocusingMRR = 3000.f;//MovementRotationRate

	float MoveDefaultSpeed = 400.f;
	float MoveRunSpeed = 600.f;
	float MoveAimingSpeed = 320.f;
	float MoveDefaultSpeed_Crouch = 250.f;
	float MoveAimingSpeed_Crouch = 200.f;
	FTimerHandle TimerHandle;

	/**상호작용 검사를 관리할 타이머 핸들*/
	FTimerHandle InteractTimerHandle;

	UPROPERTY()
	AActor* HitResultObject;
	UPROPERTY()
	AActor* PrevHitResultObject;

	UPROPERTY()
	ANonPlayerCharacterBase* NonPlayerCharacter;

	UPROPERTY()
	UCPP_QuestSubsystem* QuestSubsystem;

	/**For get Grab& Release Func*/
	UPROPERTY()
	UGrabber* GraberComponent;

	/**item trace*/
	UPROPERTY(EditAnywhere, Category = "PlayerValue")
	float ShowItemDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "PlayerValue")
	float ShowItemRadius = 100.f;

	//bool bCanSearchObject = false;

	FCollisionQueryParams Params;

	/**camera*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
	float MouseRate = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
	float AimingMouseRate = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
	float HipMouseRate = 50;

	bool bAiming = false;

	/**crosshair*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraManager* CameraManager;

	/**widget*/
	UPROPERTY()
	UMainPanelWidget* MainPanelWidget;
	UPROPERTY()
	TArray<ACPP_DamageActor*> DamageUIActors;
	int32 NextUI = 0;


	/**inventory*/
	UPROPERTY(VisibleAnywhere)
	UInventory* GameInventory;

	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
	uint8 InventoryAmountOfSlot = 20;
	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
	uint8 InventoryRowSize = 5;
	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
	float InventoryMaxWeight = 250.f;

	bool isVisible = true;
	bool IsQuestListVisible = false;

};
