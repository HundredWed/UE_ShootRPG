#include "Object/Mover.h"
#include "Kismet/GameplayStatics.h"

#include "NPC/NonPlayerCharacterBase.h"


UMover::UMover()
{	
	PrimaryComponentTick.bCanEverTick = false;
}

void UMover::BeginPlay()
{
	Super::BeginPlay();
	AActor* owner = GetOwner();
	OwnerLocOri = owner->GetActorLocation();
	OwnerRotOri = owner->GetActorRotation();
}

void UMover::MoveDown()
{
	TagetPos = GetOwner()->GetActorLocation() + MoveOffset;
	MoveStart();
}

void UMover::MoveUp()
{
	MoveDirChange();
	TagetPos = GetOwner()->GetActorLocation() + MoveOffset;
	MoveStart();
}

void UMover::MoveDirChange()
{
	const float dir = -1.f;
	MoveOffset *= dir;
}

void UMover::MoveStart()
{
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("Move")));
	FVector currentPos = GetOwner()->GetActorLocation();
	const float speed = MoveOffset.Length() / MoveTime;
	FVector newPos = FMath::VInterpConstantTo(currentPos, TagetPos, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), speed);
	GetOwner()->SetActorLocation(newPos);

	if ((int32)currentPos.Z == (int32)TagetPos.Z)
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, FString::Printf(TEXT("%f / %f"), currentPos.Z, newPos.Z));
		//GetOwner()->SetActorHiddenInGame(true);

		ANonPlayerCharacterBase* npc = Cast<ANonPlayerCharacterBase>(GetOwner());

		if (MoveOffset.Z < 0 && IsValid(npc))
		{
			FVector respawnLoc = FVector{ OwnerLocOri.X, OwnerLocOri.Y, TagetPos.Z };
			GetOwner()->SetActorLocation(respawnLoc);
			GetOwner()->SetActorRotation(OwnerRotOri);
			//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("clear")));

			FinishDown.Execute();
		}
		else if (MoveOffset.Z > 0 && IsValid(npc))
		{
			FinishUp.Execute();
			MoveDirChange();
		}
		return;
	}

	UWorld* world = GetWorld();
	if (!IsValid(world))
		return;

	FTimerHandle TimerHandle;
	world->GetTimerManager().SetTimer(TimerHandle, this, &UMover::MoveStart, 0.01f, false);
}


