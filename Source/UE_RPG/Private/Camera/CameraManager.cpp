#include "Camera/CameraManager.h"
#include "GameFramework/SpringArmComponent.h"

UCameraManager::UCameraManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	PrimaryComponentTick.bCanEverTick = true;
	NewValue = 0;
	InterpSpeed = 15.f;
}

void UCameraManager::BeginPlay()
{
	Super::BeginPlay();

}



void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(SpringArm))
	{
		UpdateSpringArmOffset(NewValue, DeltaTime);
	}
}

void UCameraManager::UpdateSpringArmOffset(float NewYoffset, float DeltaTime)
{
	SpringArm->SocketOffset.Y = FMath::FInterpTo(SpringArm->SocketOffset.Y, NewYoffset, DeltaTime, InterpSpeed);
}

