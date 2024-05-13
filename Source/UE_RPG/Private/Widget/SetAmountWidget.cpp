// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SetAmountWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Inventory.h"


void USetAmountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnClicked.AddDynamic(this, &USetAmountWidget::ClickCancel);
	ConfirmButton->OnClicked.AddDynamic(this, &USetAmountWidget::ClickConfirml);
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
	SetCnountText(SetAmountCount);
}

void USetAmountWidget::IncreaseCount()
{
	SetAmountCount = FMath::Clamp(SetAmountCount + 1, 1, MaxAmountCount);
	SetCnountText(SetAmountCount);
}

void USetAmountWidget::DecreaseCount()
{
	SetAmountCount = FMath::Clamp(SetAmountCount - 1, 1, MaxAmountCount);
	SetCnountText(SetAmountCount);
}

void USetAmountWidget::ClickCancel()
{
	InventoryRef->InventoryWidget->SetPanelEnabled(true);
	this->SetVisibility(ESlateVisibility::Hidden);
}

void USetAmountWidget::ClickConfirml()
{
	if (bThrowWidget)
	{
		InventoryRef->RemoveItemAtIndex(InventoryIndex, SetAmountCount);
	}
	else
	{
		InventoryRef->SplitStackToIndex(InventoryIndex, ToIndex, SetAmountCount);
	}

	InventoryRef->InventoryWidget->SetPanelEnabled(true);
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
		SetCnountText(SetAmountCount);
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
		SetCnountText(SetAmountCount);
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
void USetAmountWidget::SetCnountText(const int32 count)
{
	FText textcount = FText::AsNumber(count);
	TextCount->SetText(textcount);
}