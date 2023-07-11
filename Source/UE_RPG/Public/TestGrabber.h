// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "TestGrabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UTestGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	UTestGrabber();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
		void Grab();

	UFUNCTION(BlueprintCallable)
		void Release();
		
private:
	UPROPERTY(EditAnyWhere)
		float MaxLine = 200;

	UPROPERTY(EditAnyWhere)
		float GrabRadius = 100;

	UPROPERTY(EditAnyWhere)
		float HandleDis = 200;

	UPhysicsHandleComponent* GetPhysicsHandle() const;
};
