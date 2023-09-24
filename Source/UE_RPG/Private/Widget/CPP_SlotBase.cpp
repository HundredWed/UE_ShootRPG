// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_SlotBase.h"
#include "Components/Border.h"
#include "Widget/SlotDrag.h"
#include "Components/Image.h"
#include "Item/Item.h"
#include "Widget/TootipWidget.h"


bool UCPP_SlotBase::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		//UE_LOG(LogTemp, Display, TEXT("DragOver "));
		return true;
	}
	else
	{
		USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
		if (dragSlot)
		{
			UE_LOG(LogTemp, Display, TEXT("DragOver"));
			bDraggedOver = true;
			//border
			SlotBorder->SetBrushColor(FLinearColor::Gray);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("???"));
			return false;
		}
	}
}

void UCPP_SlotBase::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
		if (dragSlot)
		{
			bDraggedOver = false;
			//border
			SlotBorder->SetBrushColor(DefaultBorderColor);
		}
	}
}

void UCPP_SlotBase::InactiveSlot()
{
	/**set icon*/
	ItemIcon->SetIsEnabled(false);
	ItemIcon->SetToolTip(nullptr);
	ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	/**set border*/
	SlotBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SlotBorder->SetBrushColor(DefaultBorderColor);
}

void UCPP_SlotBase::ActiveSlot()
{
	/**set icon*/
	ItemIcon->SetIsEnabled(true);
	ItemIcon->SetBrushFromTexture(ItemRef->IconTexture);
	ItemIcon->SetVisibility(ESlateVisibility::Visible);
	/**set border*/
	SlotBorder->SetBrushColor(FLinearColor::White);
}

void UCPP_SlotBase::SetSlotToolTip()
{
	if (IsValid(ToolTip))
	{
		/**if created tootip before, don't create widget and update that tootip*/
		ToolTip->SetTootipItemRef(ItemRef);
		ToolTip->UpdateToolTip();

		ItemIcon->SetToolTip(ToolTip);
	}
	else
	{
		/**CreateWidget only once*/
		if (TootipWidgetClass)
		{
			ToolTip = CreateWidget<UTootipWidget>(GetWorld(), TootipWidgetClass);
			ToolTip->SetTootipItemRef(ItemRef);
			ToolTip->UpdateToolTip();
			ItemIcon->SetToolTip(ToolTip);
		}
	}
}


