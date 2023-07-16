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

	//Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		class UAnimMontage* EquipMontage;


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

	void GetViewPointVector(FVector& Location, FRotator& Rotation);

	void SmoothSpringArmOffset(float NewYoffset, bool bOrientRotationToMovement);

	bool PressKey(const FInputActionValue& Value);
	class AWeapon* isWeapon(AActor* hitobject) const;

	//CharacterState
    void SetStateEquiped();
	void SetStateUnEquiped();

	//Montage
	void PlayEquipMontage(FName NotifyName);

	UFUNCTION(BlueprintCallable)
		void HoldWeapon();

	UFUNCTION(BlueprintCallable)
		void UnHoldWeapon();

	FORCEINLINE ECharacterStateTypes GetCharacterState() const { return CharacterState; }
	FORCEINLINE void SetHitResultObject(AActor* hitresultobject);
	FORCEINLINE void RemoveHitResultObject();
	

private:

	ECharacterStateTypes CharacterState = ECharacterStateTypes::UnEquiped;

	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "PlayerValue")
		float ShowItemRadius = 100.f;

	AActor* HitResultObject;
	FCollisionQueryParams Params;

	class UGrabber* GraberComponent;
	class AWeapon* EquipedWeapin;
};
