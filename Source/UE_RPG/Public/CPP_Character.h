#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterStates.h"
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

	/**spring arm*/
	UPROPERTY(EditAnywhere, Category = "EditValue")
		float SpringArmSocketOffsetYValue = 100.f;

	/**Item search issue overlap counting*/
	uint8 OverlapCount = 0;

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
	void InventoryVisibility(const FInputActionValue& Value);

	void GetViewPointVector(FVector& Location, FRotator& Rotation);

	void SmoothSpringArmOffset(float NewYoffset, bool bOrientRotationToMovement);

	bool PressKey(const FInputActionValue& Value);
	class AWeapon* isWeapon(AActor* hitobject) const;
	void PickUpWeapon(class AWeapon* weapon);

	void ResetHitResultState();
	void RemoveHitResultObject();

	/**CharacterState*/
    void SetStateEquiped();
	void SetStateUnEquiped();
	bool CanAttackState();
	bool CanEquipState();
	bool CanUnEquipState();

	/**Montage*/
	void PlayEquipMontage(FName NotifyName);
	void PlayFireMontage(UAnimMontage* montage);

	UFUNCTION(BlueprintCallable)
		void HoldWeapon();

	UFUNCTION(BlueprintCallable)
		void UnHoldWeapon();

	UFUNCTION(BlueprintCallable)
		void EquippingEnd();

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
	
	/**inventory*/

	void HideGameInventory();
	void ShowGameInventory();

	FORCEINLINE class UInventory* GetInventory() { return GameInventory; }
	FORCEINLINE const uint8 GetInventorySize() { return InventoryAmountOfSlot; }
	FORCEINLINE const uint8 GetInventoryRowSize() { return InventoryRowSize; }
	

private:

	float ClampRnage(float value);

	/**character states*/
	ECharacterStateTypes CharacterState = ECharacterStateTypes::UnEquiped;
	ECharacterActionState ActionState = ECharacterActionState::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player states",  meta = (AllowPrivateAccess = "true"))
		int32 Health = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player states",  meta = (AllowPrivateAccess = "true"))
		int32 Mana = 0;

	float MoveDelfaultSpeed = 400.f;
	float MoveRunSpeed = 600.f;
	float MoveAimingSpeed = 320.f;
	float MoveDelfaultSpeed_Crouch = 250.f;
	float MoveAimingSpeed_Crouch = 200.f;

	UPROPERTY()
		class APickUpItem* HitResultObject;
	UPROPERTY()
		class APickUpItem* PrevHitResultObject;

	/**For get Grab& Release Func*/
	UPROPERTY()
		class UGrabber* GraberComponent;

	/**check 'is EquipedWeapon?' for  HoldWeapon& UnHoldWeapon Func*/
	UPROPERTY()
		class AWeapon* EquipedWeapon;

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


	/**inventory*/
	UPROPERTY(VisibleAnywhere)
		class UInventory* GameInventory;

	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
		uint8 InventoryAmountOfSlot = 20;
	UPROPERTY(EditAnywhere, Category = "InventoryComponent", meta = (AllowPrivateAccess = "true"))
		uint8 InventoryRowSize = 5;

	bool isVisible = true;
};
