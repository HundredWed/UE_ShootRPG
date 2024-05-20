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

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DamageAnimRight;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DamageAnimLeft;

	void UpdateWidget(const int32 amount);
};
