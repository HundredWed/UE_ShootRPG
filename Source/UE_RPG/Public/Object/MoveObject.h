

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveObject.generated.h"

UCLASS()
class UE_RPG_API AMoveObject : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMoveObject();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetMoverCompnent(class UMover* NewMover);

private:

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class USceneComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class UBoxComponent* KeyBoxComponent;

	/**compare gate tag with key tag */
	UPROPERTY(EditAnyWhere)
		FName ActorTag;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	/**mover component*/
	UPROPERTY()
		class UMover* Mover;

	void AttachKey();
	AActor* GetAcceptableActor() const;

	bool isPlayerIn = false;
};
