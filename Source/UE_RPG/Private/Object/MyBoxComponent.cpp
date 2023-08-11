#include "Object/MyBoxComponent.h"
#include "Object/Mover.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "CPP_Character.h"


UMyBoxComponent::UMyBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

    AttachKey = CreateDefaultSubobject<USceneComponent>(TEXT("Attach Key"));
    AttachKey->SetupAttachment(this);

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Area"));
    SphereComponent->SetupAttachment(this);
}

void UMyBoxComponent::BeginPlay()
{
    Super::BeginPlay();

    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UMyBoxComponent::OnOverlap);
    SphereComponent->OnComponentEndOverlap.AddDynamic(this, &UMyBoxComponent::OnEndOverlap);
}

void UMyBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsValid(Mover) == false)
    {
        return;
    }
       
    if (isPlayerIn)
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

            Actor->AttachToComponent(AttachKey, FAttachmentTransformRules::KeepWorldTransform);
            Mover->SetShouldMove(true);
        }
        else
        {
            Mover->SetShouldMove(false);
        }
    }

    
}

void UMyBoxComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
}


AActor* UMyBoxComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    for (AActor* Actor : Actors)
    {
        bool KeyTag = Actor->ActorHasTag(ActorTag);
        bool GrabbedTag = Actor->ActorHasTag("Grabbed");

        if (KeyTag && !GrabbedTag)
        {
            return Actor;
        }
    }
    return nullptr;
}

void UMyBoxComponent::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACPP_Character* player = Cast<ACPP_Character>(OtherActor);
    if (player)
    {
        isPlayerIn = true;
    }
}

void UMyBoxComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    isPlayerIn = false;
}