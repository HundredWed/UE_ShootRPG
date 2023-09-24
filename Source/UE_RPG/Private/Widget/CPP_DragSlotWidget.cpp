// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_DragSlotWidget.h"
#include "Item/Item.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCPP_DragSlotWidget::UpdataWidget(const UItem* item, uint32 amount)
{
	if (IsValid(item))
	{
		ItemIcon->SetBrushFromTexture(item->IconTexture);
		ItemIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (item->bCanStacked && amount > 0)
		{
			TextAmount->SetText(FText::Format(NSLOCTEXT("CPP_Slot", "TextAmount", "x{0}"), amount));
		}
		else
		{
			TextAmount->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
