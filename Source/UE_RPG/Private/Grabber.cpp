#include "Grabber.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
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
	if (PhysicsHandle == nullptr)
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
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult Hitresult;

	bool OnHit = GetGrabbableInReach(Hitresult);

	if (OnHit)
	{
		UPrimitiveComponent* HitComponent = Hitresult.GetComponent();
		WakeUp(HitComponent);
		AActor* HitActor = Hitresult.GetActor();
		HitActor->Tags.Add("Grabbed");
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			Hitresult.ImpactPoint,
			GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
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
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle is Not!!"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("succeed"));
	return PhysicsHandle;
}

void UGrabber::WakeUp(UPrimitiveComponent* HitComponent)
{
	HitComponent->SetSimulatePhysics(true);
	HitComponent->WakeAllRigidBodies();
}