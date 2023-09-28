// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_DragSlotWidget.generated.h"

/**
 * the slot visual when Drag Even. 
 */
UCLASS()
class UE_RPG_API UCPP_DragSlotWidget : public UCustomUMGWidget
{
	GENERATED_BODY()

public:
	void UpdataWidget(const class UItem* item, uint32 amount = 0);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextAmount;
};
