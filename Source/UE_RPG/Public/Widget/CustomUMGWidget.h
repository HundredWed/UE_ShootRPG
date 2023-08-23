// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomUMGWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCustomUMGWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
		class ACPP_Character* PlayerRef;

	UPROPERTY()
		class UInventory* InventoryRef;
};
