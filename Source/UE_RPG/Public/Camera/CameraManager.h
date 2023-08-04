#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(EditAnywhere)
		float InterpSpeed;

	float NewValue;
		

	FORCEINLINE void SetSpringArm(class USpringArmComponent* cameraboom) { SpringArm = cameraboom; }
	FORCEINLINE void SetCamera(class UCameraComponent* followcamera) { Camera = followcamera; }
};
