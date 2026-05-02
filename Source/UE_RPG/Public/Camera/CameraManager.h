#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStates.h"
#include "CameraManager.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ACPP_Character;
class ACameraActor;

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
	USpringArmComponent* SpringArm;
	UPROPERTY()
	UCameraComponent* Camera;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateSpringArmYOffset(float newYoffset, float deltaTime);
	void SmoothCameraFOV(float DeltaTime);

	void UpdateSpringArmZOffset(float newYoffset,float deltaTime);
	void SpringArmZOffsetFix(float deltaTime);

	void StartDialogueCamera(AActor* TargetNPC, const FTransform& transform);
	void EndDialogueCamera();

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
	ACPP_Character* MyCharacter;

	bool isAiming = false;
	ECharacterStateTypes CharacterState;

	FORCEINLINE void SetSpringArm(class USpringArmComponent* cameraboom) { SpringArm = cameraboom; }
	FORCEINLINE void SetCamera(class UCameraComponent* followcamera) { Camera = followcamera; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float CameraZoomedFOV = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed = 0;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DialogueCameraSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (AllowPrivateAccess = "true"))
	float DialogueCameraBlendOut = 2.f;

	TWeakObjectPtr<ACameraActor> CurrentDialogueCamera;
};
