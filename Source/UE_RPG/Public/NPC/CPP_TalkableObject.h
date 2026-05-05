#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/CPP_InteractInterface.h"
#include "CPP_TalkableObject.generated.h"

class UCameraComponent;

UCLASS()
class UE_RPG_API ACPP_TalkableObject : public AActor, public ICPP_InteractInterface
{
	GENERATED_BODY()
	
public:	
	ACPP_TalkableObject();
	virtual void OnConstruction(const FTransform& Transform) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* DummyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* DialogueCameraPreview;


	virtual void RequestInteract(AActor* interactor) override;
	virtual ECharacterTypes GetType() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	FTransform CachedCameraTransform;

	UPROPERTY(EditAnyWhere, Category = "NPC Info")
	FName NPCID;

public:	
	virtual void Tick(float DeltaTime) override;


	

};
