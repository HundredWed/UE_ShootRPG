#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterStates.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_NPCAnimInstance.generated.h"


enum CombatBoxNumber
{
	BoxIndex0,
	BoxIndex1,
};

UCLASS()
class UE_RPG_API UCPP_NPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;


	UPROPERTY(BlueprintReadOnly, Category = Movement)
		class UCharacterMovementComponent* MyCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float GroundSpeed = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float Angle = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float MovementYawOffset = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float MovementPrevYawOffset = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float RootYawOffset = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float Pich = 0;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		FVector Velocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool isMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterState")
		ENPCState NPCState = ENPCState::Normal;

	float GetAngle2();
	void SetNPCState(ENPCState state) { NPCState = state; }

	/**AnimNotify_*/
	UFUNCTION()
		void AnimNotify_ActivateCombatBox_0(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_DeactivateCombatBox_0(UAnimNotify* Notify);

	UFUNCTION()
		void AnimNotify_ActivateCombatBox_1(UAnimNotify* Notify);
	UFUNCTION()
		void AnimNotify_DeactivateCombatBox_1(UAnimNotify* Notify);

	UFUNCTION()
		void AnimNotify_TriggerIsDirectly(UAnimNotify* Notify);

	UFUNCTION()
		void AnimNotify_ShootProjectile(UAnimNotify* Notify);

	UFUNCTION()
		void AnimNotify_KnockBackCharacter(UAnimNotify* Notify);

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class ANonPlayerCharacterBase* NPC;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class AEnemyBase* TypEnemy;
};
