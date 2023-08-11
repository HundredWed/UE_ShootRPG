#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveStart(float deltatime);
	void SetShouldMove(bool isMove);


	UPROPERTY(EditAnywhere)
		bool moving = false;

	UPROPERTY(EditAnywhere)
		FVector MoveOffset = FVector::Zero();
		
	UPROPERTY(EditAnywhere)
		FVector TagetPos = FVector::Zero();

	UPROPERTY(EditAnywhere)
		float MoveTime = 4;

	FVector Pos = FVector::Zero();

private:
		
};
