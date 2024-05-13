#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_Character.generated.h"


UCLASS()
class UE_RPG_API ACPP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ACPP_Character();
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	/**Input*/
	UPROPERTY(EditAnywhere, Category = Input)
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* SpeedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* GrabAndPickupAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* DodgeToggle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* AimingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* InventoryToggle;


	/**Montage*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* AimingFireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* DodgeMontage;

	/**main panel widget*/
	UPROPERTY(EditAnywhere, Category = "Main Widget")
		TSubclassOf< class UMainPanelWidget> MainPanelclass;

	/**spring arm*/
	UPROPERTY(EditAnywhere, Category = "EditValue")
		float SpringArmSocketOffsetYValue = 100.f;

	/**Item search issue overlap counting*/
	uint8 OverlapCount = 0;

	/**key down info*/
	bool bShiftDown = false;/*for inventory splite*/

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SreachItem();
	void ObjectSearchTrace();
	bool SetShpereTrace(FHitResult& HitResult);

	/**Input*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SetSpeed(const FInputActionValue& Value);
	void GrabItem(const FInputActionValue& Value);
	void PickUp(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Aiming(const FInputActionValue& Value);
	void SetCrouch(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void InventoryVisibility(const FInputActionValue& Value);

	/**trace*/
	void GetViewPointVector(FVector& Location, FRotator& Rotation);

	/**camera*/
	void SmoothSpringArmOffset(float NewYoffset);

	bool PressKey(const FInputActionValue& Value);
	class AWeapon* isWeapon(AActor* hitobject) const;
	bool PickUpWeapon();
	bool IsInActivePrevEquippedWeapon();
	void ActionWeaponAbility();
	void FireWeapon();
	void CanTrigger();
	void SetMovementRotate(bool bORT, float rotationRate);
	void LookAt();
	

	void ResetHitResultState();
	void RemoveHitResultObject();

	/**CharacterState*/
    void SetStateEquipped();
	void SetStateUnEquipped();
	void SetEquippedWeapon(class AWeapon* equippedWeapon);
	class AWeapon* GetEquippedWeapon();
	bool CanAttackState();
	bool CanEquipState();
	bool CanUnEquipState();
	bool IsValidEquipWeapon();
	void SetWeaponAbility(const uint8 id);
	void SetFireRate(float rate);

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
		void DodgeEnd();

	/**mouse*/
	void SetMouseRate();
	void CalculateCrosshairSpread(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE ECharacterStateTypes GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetHitResultObject(class APickUpItem* hitresultobject);
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	FORCEINLINE void SetCanSearchObject(bool cansearch) { bCanSearchObject = cansearch; }

	FORCEINLINE void IncreasePlayerHP(const uint32 value) { Health += value; }
	FORCEINLINE void DecreasePlayerHP(const uint32 value) { Health -= value; }
	FORCEINLINE int32 GetPlayerATK() { return PlayerATK; }
	FORCEINLINE bool GetPlayerMoveState() { return bMoving; }
	
	/**inventory*/
	void HideGameInventory();
	void ShowGameInventory();
	const float GetPlayerWeightInfo() { return InventoryMaxWeight; }

	FORCEINLINE class UInventory* GetInventory() { return GameInventory; }
	FORCEINLINE const int16 GetInventorySize() { return InventoryAmountOfSlot; }
	FORCEINLINE const int16 GetInventoryRowSize() { return InventoryRowSize; }

private:

	float ClampRnage(float value);

	/**character states*/
	ECharacterStateTypes CharacterState = ECharacterStateTypes::UnEquiped;
	ECharacterActionState ActionState = ECharacterActionState::Normal;

	/**character states - weapon*/
	bool PressFireKey = false;
	bool bTrigger = true;
	bool bMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states",  meta = (AllowPrivateAccess = "true"))
		int32 Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states",  meta = (AllowPrivateAccess = "true"))
		int32 Mana = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
		int32 Stamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states",  meta = (AllowPrivateAccess = "true"))
		int32 PlayerATK = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
		float FireRate = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
		float TriggerRate = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
		float DefaultMRR = 500.f;//MovementRotationRate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player states", meta = (AllowPrivateAccess = "true"))
		float FocusingMRR = 3000.f;//MovementRotationRate

	float MoveDelfaultSpeed = 400.f;
	float MoveRunSpeed = 600.f;
	float MoveAimingSpeed = 320.f;
	float MoveDelfaultSpeed_Crouch = 250.f;
	float MoveAimingSpeed_Crouch = 200.f;
	FTimerHandle TimerHandle;


	/**weapon*/
	int16 AbilityId = -1;

	UPROPERTY()
		class APickUpItem* HitResultObject;
	UPROPERTY()
		class APickUpItem* PrevHitResultObject;
	UPROPERTY()
		class ARifle* Rifle;

	/**For get Grab& Release Func*/
	UPROPERTY()
		class UGrabber* GraberComponent;

	/**check 'is EquipedWeapon?' for  HoldWeapon& UnHoldWeapon Func*/
	UPROPERTY()
		class AWeapon* EquippedWeapon = nullptr;

	/**item trace*/
	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemRadius = 100.f;

	bool bCanSearchObject = false;

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
		class UCameraManager* CameraManager;

	/**widget*/
	UPROPERTY()
		class UMainPanelWidget* MainPanelWidget;


	/**inventory*/
	UPROPERTY(VisibleAnywhere)
		class UInventory* GameInventory;

	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
		uint8 InventoryAmountOfSlot = 20;
	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
		uint8 InventoryRowSize = 5;
	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
		float InventoryMaxWeight = 250.f;

	bool isVisible = true;

	UPROPERTY()
		TMap<uint8, class UWeaponAbilityBase*> WeaponAbility;
	UPROPERTY()
		TSet<uint8> WeaponAbilityStorage;

};
