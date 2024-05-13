// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_DamageUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_DamageUI : public UUserWidget
{
	GENERATED_BODY()

public:	
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DamageText;

	void UpdateWidget(const int32 amount);
};
