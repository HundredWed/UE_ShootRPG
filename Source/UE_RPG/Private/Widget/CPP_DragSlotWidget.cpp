// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_DragSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCPP_DragSlotWidget::UpdataWidget(const FItemInfoTable* itemData, uint32 amount)
{
	if (itemData)
	{
		ItemIcon->SetBrushFromTexture(itemData->IconTexture);
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (itemData->bCanStacked && amount > 0)
		{
			TextAmount->SetText(FText::Format(NSLOCTEXT("CPP_DragSlotWidget", "TextAmount", "x{0}"), amount));
			TextAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			TextAmount->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
