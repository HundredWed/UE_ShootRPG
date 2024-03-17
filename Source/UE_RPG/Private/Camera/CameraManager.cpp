#include "Camera/CameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_Character.h"
#include "Kismet/GameplayStatics.h"

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

		/**feel sick due to inaccuracy in Aim and Motion fix*/
		SpringArmZOffsetFix(DeltaTime);
	}
	if (IsValid(SpringArm) && IsValid(Camera))
	{
		UpdateSpringArmYOffset(NewValue, DeltaTime);
		SmoothCameraFOV(DeltaTime);
	}	
}

void UCameraManager::UpdateSpringArmYOffset(float newYoffset, float deltaTime)
{
    
	SpringArm->SocketOffset.Y = FMath::FInterpTo(SpringArm->SocketOffset.Y, newYoffset, deltaTime, InterpSpeed);
}

void UCameraManager::UpdateSpringArmZOffset(float newZoffset,float deltaTime)
{
	SpringArm->SocketOffset.Z = FMath::FInterpTo(SpringArm->SocketOffset.Z, newZoffset, deltaTime, 7.5f);
}

void UCameraManager::SpringArmZOffsetFix(float deltaTime)
{
	/**a fixed number '32.f' and 0*/
	if (MyCharacter->GetMovementComponent()->IsCrouching() && MyCharacter->GetMovementComponent()->Velocity.Length() > 0)
	{
		UpdateSpringArmZOffset(32.f, deltaTime);
	}
	else
	{
		UpdateSpringArmZOffset(0, deltaTime);
	}
}

void UCameraManager::SmoothCameraFOV(float DeltaTime)
{
	if (isAiming && CharacterState != ECharacterStateTypes::UnEquiped)
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

