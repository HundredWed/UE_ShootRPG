// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_EnemyCombatBox.generated.h"

UCLASS()
class UE_RPG_API ACPP_EnemyCombatBox : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACPP_EnemyCombatBox();

	UPROPERTY(EditAnywhere)
		class UBoxComponent* CombatBox;

protected:
	
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;

public:
	

};
