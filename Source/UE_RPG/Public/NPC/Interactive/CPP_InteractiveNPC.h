// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NonPlayerCharacterBase.h"
#include "Interface/CPP_InteractInterface.h"
#include "CPP_InteractiveNPC.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API ACPP_InteractiveNPC : public ANonPlayerCharacterBase
{
	GENERATED_BODY()

public:

	ACPP_InteractiveNPC();
	virtual void OnConstruction(const FTransform& Transform) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* DummyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* DialogueCameraPreview;


	virtual void RequestInteract(AActor* interactor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	FTransform CachedCameraTransform;

private:

	void InitQuestSystem();
};
