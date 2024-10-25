#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE_RPG/UtilityMecro.h"
#include "Mover.generated.h"

DECLARE_DELEGATE(FFinishMoveDownDelegate);
DECLARE_DELEGATE(FFinishMoveUpDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMover();
protected:
	virtual void BeginPlay() override;
	void MoveStart();

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void MoveDown();
	void MoveUp();
	void MoveDirChange();

	FFinishMoveDownDelegate FinishDown;
	FFinishMoveUpDelegate FinishUp;
	
	UPROPERTY(EditAnywhere)
		FVector MoveOffset = FVector::Zero();

	UPROPERTY(EditAnywhere)
		float MoveTime = 4.f;

private:

	UPROPERTY(VisibleAnywhere)
	FVector TagetPos = FVector::Zero();
	FVector OwnerLocOri = FVector::Zero();
	FRotator OwnerRotOri = FRotator::ZeroRotator;
};
