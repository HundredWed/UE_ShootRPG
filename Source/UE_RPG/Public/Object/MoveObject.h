

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
	void AttachKey();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class USceneComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, Category = "Object Component")
		class UBoxComponent* KeyBoxComponent;

	/**compare gate tag with key tag */
	UPROPERTY(EditAnyWhere)
		FName ActorTag;

	
	/**mover component*/
	UPROPERTY(EditAnyWhere)
		class UMover* Mover;
	bool moving = false;

	AActor* GetAcceptableActor() const;

	/**chek Player in sphere Area for active tick*/
	bool isPlayerIn = false;
};
