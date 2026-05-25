#include "NPC/Interactive/CPP_TalkableObject.h"
#include "Camera/CameraComponent.h"

#include "CPP_Character.h"

ACPP_TalkableObject::ACPP_TalkableObject()
{
	PrimaryActorTick.bCanEverTick = false;

	DummyComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy"));
	SetRootComponent(DummyComponent);

	DialogueCameraPreview = CreateDefaultSubobject<UCameraComponent>(TEXT("DialogueCameraPreview"));
	DialogueCameraPreview->SetupAttachment(GetRootComponent());
	DialogueCameraPreview->bIsEditorOnly = true;
}

void ACPP_TalkableObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR

	if (DialogueCameraPreview)
	{
		CachedCameraTransform = DialogueCameraPreview->GetComponentTransform();
	}

#endif
}

void ACPP_TalkableObject::RequestInteract(AActor* interactor)
{
	if (ACPP_Character* character = Cast<ACPP_Character>(interactor))
	{
		character->SetDialogue(NPCID, CachedCameraTransform);
	}
}

ECharacterTypes ACPP_TalkableObject::GetType()
{
	return ECharacterTypes::NPC_Talkable;
}

void ACPP_TalkableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPP_TalkableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

