// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_SlotBase.h"
#include "Components/Border.h"
#include "Widget/SlotDrag.h"
#include "Components/Image.h"
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
		if (dragSlot && IsValidDragOverTarget(InOperation))
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
		if (dragSlot && !ItemIcon->IsVisible())
		{
			SetBorderDefault();
		}
	}
}

void UCPP_SlotBase::SetBorderDefault()
{
	bDraggedOver = false;
	SlotBorder->SetBrushColor(DefaultBorderColor);
}

void UCPP_SlotBase::SetBorderWhite()
{
	bDraggedOver = false;
	SlotBorder->SetBrushColor(FLinearColor::White);
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

void UCPP_SlotBase::ActiveSlot(UTexture2D* icon)
{
	/**set icon*/
	ItemIcon->SetIsEnabled(true);
	ItemIcon->SetBrushFromTexture(icon);
	ItemIcon->SetVisibility(ESlateVisibility::Visible);
	/**set border*/
	SlotBorder->SetBrushColor(FLinearColor::White);
}

void UCPP_SlotBase::SetSlotToolTip(const FItemInfoTable* itemInfo)
{
	if (IsValid(ToolTip))
	{
		/**if created tootip before, don't create widget and update that tootip*/
		ToolTip->InitToolTip(itemInfo);
		ItemIcon->SetToolTip(ToolTip);
	}
	else
	{
		/**CreateWidget only once*/
		if (TootipWidgetClass)
		{
			ToolTip = CreateWidget<UTootipWidget>(GetWorld(), TootipWidgetClass);
			ToolTip->InitToolTip(itemInfo);
			ItemIcon->SetToolTip(ToolTip);
		}
	}
}

void UCPP_SlotBase::SetSlotToolTip(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo)
{
	if (IsValid(ToolTip))
	{
		/**if created tootip before, don't create widget and update that tootip*/
		ToolTip->InitToolTip(itemInfo, equipmentInfo);
		ItemIcon->SetToolTip(ToolTip);
	}
	else
	{
		/**CreateWidget only once*/
		if (TootipWidgetClass)
		{
			ToolTip = CreateWidget<UTootipWidget>(GetWorld(), TootipWidgetClass);
			ToolTip->InitToolTip(itemInfo, equipmentInfo);
			ItemIcon->SetToolTip(ToolTip);
		}
	}
}


