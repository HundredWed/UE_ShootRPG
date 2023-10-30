#include "Object/Mover.h"
#include "Kismet/GameplayStatics.h"


UMover::UMover()
{	
	PrimaryComponentTick.bCanEverTick = false;
}


void UMover::BeginPlay()
{
	Super::BeginPlay();

	Pos = GetOwner()->GetActorLocation();
	TagetPos = Pos + MoveOffset;
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
		
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, TEXT("MoveStart"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.001f, false);
}


