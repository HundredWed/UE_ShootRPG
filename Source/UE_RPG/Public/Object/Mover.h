#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE_RPG/UtilityMecro.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMover();

protected:
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintCallable)
		void MoveStart();


	UPROPERTY(EditAnywhere)
		FVector MoveOffset = FVector::Zero();
		
	UPROPERTY(VisibleAnywhere)
		FVector TagetPos = FVector::Zero();

	UPROPERTY(EditAnywhere)
		float MoveTime = 4;

	UPROPERTY(VisibleAnywhere)
		FVector Pos = FVector::Zero();

private:
		
};
