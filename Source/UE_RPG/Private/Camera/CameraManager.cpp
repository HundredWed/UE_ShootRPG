#include "Camera/CameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPP_Character.h"

UCameraManager::UCameraManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	//spring arm
	NewValue = 0;
	InterpSpeed = 15.f;

	//camera
	CameraZoomedFOV = 35.f;
	ZoomInterpSpeed = 20.f;
}

void UCameraManager::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<ACPP_Character>(GetOwner());
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay!!"));
	
}



void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MyCharacter)
	{
		isAiming = MyCharacter->GetIsAiming();
		CharacterState = MyCharacter->GetCharacterState();
	}
	if (IsValid(SpringArm) && IsValid(Camera))
	{
		UpdateSpringArmOffset(NewValue, DeltaTime);
		SmoothCameraFOV(DeltaTime);
		//UE_LOG(LogTemp, Warning, TEXT("Tick!!"));
	}
}

void UCameraManager::UpdateSpringArmOffset(float NewYoffset, float DeltaTime)
{
	SpringArm->SocketOffset.Y = FMath::FInterpTo(SpringArm->SocketOffset.Y, NewYoffset, DeltaTime, InterpSpeed);
}

void UCameraManager::SmoothCameraFOV(float DeltaTime)
{
	if (isAiming && CharacterState == ECharacterStateTypes::Equiped)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	Camera->SetFieldOfView(CameraCurrentFOV);
}

void UCameraManager::SetBeginCamera()
{
	if (IsValid(Camera))
	{
		CameraDefaultFOV = Camera->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("no camera!!"));
	}
}

