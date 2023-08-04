#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStates.h"
#include "CameraManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UCameraManager();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY()
		class USpringArmComponent* SpringArm;
	UPROPERTY()
		class UCameraComponent* Camera;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateSpringArmOffset(float NewYoffset, float DeltaTime);
	void SmoothCameraFOV(float DeltaTime);

	UFUNCTION()
		void SetBeginCamera();

	UPROPERTY(EditAnywhere)
		float InterpSpeed;

	float NewValue;
	

	//camera
	float CameraDefaultFOV = 0;
	float CameraCurrentFOV = 0;

	//character state;
	UPROPERTY()
		class ACPP_Character* MyCharacter;

	bool isAiming = false;
	ECharacterStateTypes CharacterState;

	FORCEINLINE void SetSpringArm(class USpringArmComponent* cameraboom) { SpringArm = cameraboom; }
	FORCEINLINE void SetCamera(class UCameraComponent* followcamera) { Camera = followcamera; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float CameraZoomedFOV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		float ZoomInterpSpeed = 0;

};
