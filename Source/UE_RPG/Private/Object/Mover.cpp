#include "Object/Mover.h"

UMover::UMover()
{	
	PrimaryComponentTick.bCanEverTick = true;
}


void UMover::BeginPlay()
{
	Super::BeginPlay();

	Pos = GetOwner()->GetActorLocation();
	
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Target = Pos;
	if (moving)
	{
		Target = Pos + MoveOffset;
	}
	FVector currentPos = GetOwner()->GetActorLocation();
	float speed = MoveOffset.Length() / MoveTime;

	FVector newPos = FMath::VInterpConstantTo(currentPos, Target, DeltaTime, speed);
	GetOwner()->SetActorLocation(newPos);
}

void UMover::SetShouldMove(bool isMove)
{
	moving = isMove;
}

