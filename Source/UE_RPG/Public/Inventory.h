// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UInventory();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE TArray<class AItem*> GetInventory() { return Inventory; }

private:

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<class AItem*> Inventory;
};
