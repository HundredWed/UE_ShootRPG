#include "Object/Mover.h"
#include "Kismet/GameplayStatics.h"


UMover::UMover()
{	
	PrimaryComponentTick.bCanEverTick = false;
}


void UMover::BeginPlay()
{
	Super::BeginPlay();
}


void UMover::MoveDown()
{
	TagetPos = GetOwner()->GetActorLocation() + MoveOffset;
	MoveStart();
}

void UMover::MoveStart()
{
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("Move")));
	FVector currentPos = GetOwner()->GetActorLocation();
	float speed = MoveOffset.Length() / MoveTime;
	FVector newPos = FMath::VInterpConstantTo(currentPos, TagetPos, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), speed);
	GetOwner()->SetActorLocation(newPos);

	if ((int32)currentPos.Z == (int32)TagetPos.Z)
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("%f / %f"), currentPos.Z, newPos.Z));
		GetOwner()->SetActorHiddenInGame(true);
		return;
	}

	UWorld* world = GetWorld();
	if (!IsValid(world))
		return;

	FTimerHandle TimerHandle;
	world->GetTimerManager().SetTimer(TimerHandle, this, &UMover::MoveStart, 0.01f, false);
}


