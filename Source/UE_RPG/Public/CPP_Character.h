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
		class UCharacterSpringArm* SpringArm;

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
		class UInputAction* JumppAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UInputAction* AimingAction;

	//Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* FireMontage;


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

	void GetViewPointVector(FVector& Location, FRotator& Rotation);

	void SmoothSpringArmOffset(float NewYoffset, bool bOrientRotationToMovement);
	void SmoothCameraFOV(float DeltaTime);

	bool PressKey(const FInputActionValue& Value);
	class AWeapon* isWeapon(AActor* hitobject) const;

	//CharacterState
    void SetStateEquiped();
	void SetStateUnEquiped();
	bool CanAttackState();

	//Montage
	void PlayEquipMontage(FName NotifyName);
	void PlayFireMontage();

	UFUNCTION(BlueprintCallable)
		void HoldWeapon();

	UFUNCTION(BlueprintCallable)
		void UnHoldWeapon();

	UFUNCTION(BlueprintCallable)
		void EquippingEnd();

	FORCEINLINE ECharacterStateTypes GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetHitResultObject(AActor* hitresultobject);
	FORCEINLINE void RemoveHitResultObject();
	

private:

	ECharacterStateTypes CharacterState = ECharacterStateTypes::UnEquiped;
	ECharacterActionState ActionState = ECharacterActionState::Normal;

	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemRadius = 100.f;

	bool bAiming = false;
	float CameraDefaultFOV = 0;
	float CameraCurrentFOV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float CameraZoomedFOV = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float ZoomInterpSpeed = 0;

	UPROPERTY()
	AActor* HitResultObject;

	FCollisionQueryParams Params;

	UPROPERTY()
	class UGrabber* GraberComponent;

	UPROPERTY()
	class AWeapon* EquipedWeapon;
};
