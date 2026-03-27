// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SetAmountWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Inventory.h"
#include "Widget/CPP_InventoryWidget.h"


void USetAmountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnClicked.AddDynamic(this, &USetAmountWidget::ClickCancel);
	ConfirmButton->OnClicked.AddDynamic(this, &USetAmountWidget::ClickConfirm);
	MinusButton->OnClicked.AddDynamic(this, &USetAmountWidget::ClickMinus);
	PlusButton->OnClicked.AddDynamic(this, &USetAmountWidget::ClickPlus);
}

void USetAmountWidget::InitWidgetInfo(const int32 amount, const int16 index, bool bthrowEvent, const int16 toIndex)
{
	MaxAmountCount = amount;
	InventoryIndex = index;
	bThrowWidget = bthrowEvent;
	ToIndex = toIndex;

	SetAmountCount = 1;
	SetCountText(SetAmountCount);
}

void USetAmountWidget::IncreaseCount()
{
	SetAmountCount = FMath::Clamp(SetAmountCount + 1, 1, MaxAmountCount);
	SetCountText(SetAmountCount);
}

void USetAmountWidget::DecreaseCount()
{
	SetAmountCount = FMath::Clamp(SetAmountCount - 1, 1, MaxAmountCount);
	SetCountText(SetAmountCount);
}

void USetAmountWidget::ClickCancel()
{
	if (!InventoryRef.IsValid())
	{
		return;
	}

	InventoryRef->InventoryWidget->SetPanelEnabled(true);//델리게이트로
	this->SetVisibility(ESlateVisibility::Hidden);
}

void USetAmountWidget::ClickConfirm()
{
	if (!InventoryRef.IsValid())
	{
		return;
	}

	if (bThrowWidget)
	{
		InventoryRef->RemoveItemAtIndex(InventoryIndex, SetAmountCount);
	}
	else
	{
		InventoryRef->SplitStackToIndex(InventoryIndex, ToIndex, SetAmountCount);
	}

	InventoryRef->InventoryWidget->SetPanelEnabled(true);//델리게이트로
	this->SetVisibility(ESlateVisibility::Hidden);
}

void USetAmountWidget::ClickMinus()
{
	ClickCount += 1;
	FTimerHandle TimerHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USetAmountWidget::ResetCount, 0.17f);

	/**double click*/
	if (ClickCount > 1)
	{
		ResetCount(); 
		SetAmountCount = 1;
		SetCountText(SetAmountCount);
	}
	else
	{
		DecreaseCount();
	}
}

void USetAmountWidget::ClickPlus()
{

	ClickCount += 1;
	FTimerHandle TimerHandle;
	GetOuter()->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USetAmountWidget::ResetCount, 0.17f);

	/**double click*/
	if (ClickCount > 1)
	{
		ResetCount();
		SetAmountCount = MaxAmountCount;
		SetCountText(SetAmountCount);
	}
	else
	{
		IncreaseCount();
	}
}

void USetAmountWidget::ResetCount()
{
	ClickCount = 0;
}
void USetAmountWidget::SetCountText(const int32 count)
{
	FText textcount = FText::AsNumber(count);
	TextCount->SetText(textcount);
}