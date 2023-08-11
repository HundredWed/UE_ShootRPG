#include "Object/Mover.h"

UMover::UMover()
{	
	PrimaryComponentTick.bCanEverTick = true;
}


void UMover::BeginPlay()
{
	Super::BeginPlay();

	Pos = GetOwner()->GetActorLocation();
	TagetPos = Pos + MoveOffset;
}

void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (moving)
	{
		MoveStart(DeltaTime);
	}
	
}

void UMover::MoveStart(float deltatime)
{
	FVector currentPos = GetOwner()->GetActorLocation();
	float speed = MoveOffset.Length() / MoveTime;
	FVector newPos = FMath::VInterpConstantTo(currentPos, TagetPos, deltatime, speed);
	GetOwner()->SetActorLocation(newPos);
}

void UMover::SetShouldMove(bool isMove)
{
	moving = isMove;
}

