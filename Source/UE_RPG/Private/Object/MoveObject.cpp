#include "Object/MoveObject.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "CPP_Character.h"
#include "Object/Mover.h"

AMoveObject::AMoveObject()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Mesh"));
	RootComponent = StaticMesh;

	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Key"));
	AttachPoint->SetupAttachment(GetRootComponent());

	KeyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Key"));
	KeyBoxComponent->SetupAttachment(GetRootComponent());
	 
	Mover = CreateDefaultSubobject<UMover>(TEXT("Moving component"));
}


void AMoveObject::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(KeyBoxComponent))
	{
		KeyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMoveObject::OnBoxOverlap);
	}
	else
	{
		WARNINGLOG(TEXT("KeyBoxComponent is not valid at MoveObject"))
	}
}

void AMoveObject::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (moving == false)
	{
		AttachKey();
	}
}

void AMoveObject::AttachKey()
{
	AActor* Actor = GetAcceptableActor();
	if (IsValid(Actor) && IsValid(Mover))
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (IsValid(Component))
		{
			/**don't move actor*/
			Component->SetSimulatePhysics(false);
		}

		Actor->SetActorLocation(AttachPoint->GetComponentLocation());
		Actor->AttachToComponent(AttachPoint, FAttachmentTransformRules::KeepWorldTransform);

		Mover->MoveDown();
		moving = true;
	}
}

AActor* AMoveObject::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	KeyBoxComponent->GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor))
		{
			bool KeyTag = Actor->ActorHasTag(ActorTag);
			bool GrabbedTag = Actor->ActorHasTag("Grabbed");

			if (KeyTag && !GrabbedTag)
			{
				return Actor;
			}
		}
	}
	return nullptr;
}



