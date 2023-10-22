// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "SetAmountWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API USetAmountWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* CancelButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* ConfirmButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* MinusButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* PlusButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextCount;

	int32 SetAmountCount = 0;
	int32 MaxAmountCount = 99;

	int16 InventoryIndex = 0;
	/**when split*/
	int16 ToIndex = -1;

	uint8 ClickCount = 0;

	bool bThrowWidget = false;

public:
	void InitWidgetInfo(const int32 amount, const int16 index, bool bthrowEvent, 
		/*when split*/const int16 toIndex = -1);
	void IncreaseCount();
	void DecreaseCount();
	void SetCnountText(const int32 count);

	UFUNCTION()
		void ClickCancel();
	UFUNCTION()
		void ClickConfirml();
	UFUNCTION()
		void ClickMinus();
	UFUNCTION()
		void ClickPlus();

	UFUNCTION()
		void ResetCount();
};
