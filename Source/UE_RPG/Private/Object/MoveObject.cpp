#include "Object/MoveObject.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "CPP_Character.h"
#include "Object/Mover.h"

AMoveObject::AMoveObject()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root Mesh"));
	RootComponent = StaticMesh;

	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Key"));
	AttachPoint->SetupAttachment(GetRootComponent());

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("WakeUp Tick Area"));
	SphereComponent->SetupAttachment(GetRootComponent());

	KeyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Key"));
	KeyBoxComponent->SetupAttachment(GetRootComponent());
}


void AMoveObject::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMoveObject::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMoveObject::OnEndOverlap);
}

void AMoveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (isPlayerIn)
	{
		AttachKey();
	}

}

void AMoveObject::SetMoverCompnent(UMover* NewMover)
{
	Mover = NewMover;
}

void AMoveObject::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPP_Character* player = Cast<ACPP_Character>(OtherActor);
	if (IsValid(player))
	{
		UE_LOG(LogTemp,Display,TEXT("Player In!"))
		isPlayerIn = true;
	}
}

void AMoveObject::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACPP_Character* player = Cast<ACPP_Character>(OtherActor);
	if (IsValid(player))
	{
		UE_LOG(LogTemp, Display, TEXT("Player Out!"))
		isPlayerIn = false;
	}
}

void AMoveObject::AttachKey()
{
	AActor* Actor = GetAcceptableActor();
	if (IsValid(Actor))
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (IsValid(Component))
		{
			/**don't move actor*/
			Component->SetSimulatePhysics(false);
		}

		//Actor->AttachToComponent(AttachPoint, FAttachmentTransformRules::KeepWorldTransform);
		Actor->SetActorLocation(AttachPoint->GetComponentLocation());
		Mover->SetShouldMove(true);
	}
	else
	{
		Mover->SetShouldMove(false);
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



