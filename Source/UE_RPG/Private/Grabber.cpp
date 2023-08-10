#include "Grabber.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Item/GrabbableItem.h"
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
		return;

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		FVector Targetlocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().Vector() * HandleDistance;
		FVector Loc = FVector(Targetlocation.X, Targetlocation.Y, GetComponentLocation().Z);
		FRotator Rot = FRotator(GetComponentRotation().Pitch, 0.f, 0.f);
		PhysicsHandle->SetTargetLocationAndRotation(Loc, GetOwner()->GetActorRotation());

	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return;
	}

	FHitResult hitresult;

	bool OnHit = GetGrabbableInReach(hitresult);
	

	if (OnHit)
	{
		UE_LOG(LogTemp, Display, TEXT("OnHit grab!!!"));
		UPrimitiveComponent* HitComponent = hitresult.GetComponent();

		if (IsValid(HitComponent) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Grab HitComponent is not!!!"));
			return;
		}

		WakeUp(HitComponent);
		AActor* HitActor = hitresult.GetActor();
		if (IsValid(HitActor))
		{
			HitActor->Tags.Add("Grabbed");
			PhysicsHandle->GrabComponentAtLocationWithRotation(
				HitComponent,
				NAME_None,
				hitresult.ImpactPoint,
				GetOwner()->GetActorRotation()
			);

			AGrabbableItem* Grabbeditem = Cast<AGrabbableItem>(HitActor);
			if (IsValid(Grabbeditem))
			{
				Grabbeditem->SetStateEquiped();
			}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Grab HitActor is not!!!"));
		}
		
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (IsValid(PhysicsHandle) == false)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();

		AGrabbableItem* Grabbeditem = Cast<AGrabbableItem>(GrabbedActor);
		if (IsValid(Grabbeditem))
		{
			Grabbeditem->SetStateUnEquiped();
		}
	}
}


bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * GrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius); 

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere, Params);
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (IsValid(PhysicsHandle) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle is Not!!"));
	}

	//UE_LOG(LogTemp, Display, TEXT("succeed"));
	return PhysicsHandle;
}

void UGrabber::WakeUp(UPrimitiveComponent* HitComponent)
{
	HitComponent->SetSimulatePhysics(true);
	HitComponent->WakeAllRigidBodies();
}