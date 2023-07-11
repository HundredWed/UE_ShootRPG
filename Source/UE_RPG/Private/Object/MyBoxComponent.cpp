#include "Object/MyBoxComponent.h"
#include "Object/Mover.h"

UMyBoxComponent::UMyBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyBoxComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMyBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (Mover == nullptr)
        return;

    AActor* Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr)
        {
            Component->SetSimulatePhysics(false);//트리거 박스에 놓았을 때 그 지점에 고정.
        }

        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
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
