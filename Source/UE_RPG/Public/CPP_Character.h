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

	//Input
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

	//Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* AimingFireMontage;

	//spring arm
	UPROPERTY(EditAnywhere, Category = "EditValue")
		float SpringArmSocketOffsetYValue = 100.f;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void ObjectSearchTrace();
	bool SetShpereTrace(FHitResult& HitResult);

	//Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SetSpeed(const FInputActionValue& Value);
	void GrabItem(const FInputActionValue& Value);
	void PickUp(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Aiming(const FInputActionValue& Value);
	void SetCrouch(const FInputActionValue& Value);

	void GetViewPointVector(FVector& Location, FRotator& Rotation);

	void SmoothSpringArmOffset(float NewYoffset, bool bOrientRotationToMovement);

	bool PressKey(const FInputActionValue& Value);
	class AWeapon* isWeapon(AActor* hitobject) const;
	void PickUpWeapon(AWeapon* weapon);

	//CharacterState
    void SetStateEquiped();
	void SetStateUnEquiped();
	bool CanAttackState();

	//Montage
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
	FORCEINLINE void SetHitResultObject(AActor* hitresultobject);
	FORCEINLINE void RemoveHitResultObject();
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	FORCEINLINE void SetCanSearchObject(bool cansearch) { bCanSearchObject = cansearch; }

	

private:

	float ClampRnage(float value);

	//character states
	ECharacterStateTypes CharacterState = ECharacterStateTypes::UnEquiped;
	ECharacterActionState ActionState = ECharacterActionState::Normal;

	float MoveDelfaultSpeed = 400.f;
	float MoveRunSpeed = 600.f;
	float MoveAimingSpeed = 320.f;
	float MoveDelfaultSpeed_Crouch = 250.f;
	float MoveAimingSpeed_Crouch = 200.f;

	UPROPERTY()
		AActor* HitResultObject;

	//For get Grab & Release Func 
	UPROPERTY()
		class UGrabber* GraberComponent;

	//check 'is EquipedWeapon?' for  HoldWeapon & UnHoldWeapon Func
	UPROPERTY()
		class AWeapon* EquipedWeapon;

	//item trace
	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemRadius = 100.f;

	bool bCanSearchObject = false;

	FCollisionQueryParams Params;

	//camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float MouseRate = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float AimingMouseRate = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float HipMouseRate = 50;

	bool bAiming = false;

	//crosshair
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
	
};
