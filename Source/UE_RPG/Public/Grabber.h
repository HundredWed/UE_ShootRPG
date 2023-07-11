#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGrabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grab();
	void Release();

	
private:

	UPROPERTY(EditAnywhere, Category = "GrabValue")
		float GrabDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "GrabValue")
		float HandleDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "GrabValue")
		float GrabRadius = 100.f;


	void WakeUp(UPrimitiveComponent* HitComponent);
	class UPhysicsHandleComponent* GetPhysicsHandle() const;

	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};
