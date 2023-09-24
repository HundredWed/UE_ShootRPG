// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_EquipSlot.h"
#include "Widget/TootipWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Widget/SlotDrag.h"
#include "Item/Item.h"

void UCPP_EquipSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCPP_EquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UCPP_EquipSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry,  InDragDropEvent, InOperation);

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

void UCPP_EquipSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

bool UCPP_EquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return false;
}

FReply UCPP_EquipSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return FReply::Handled();
}

void UCPP_EquipSlot::InitEquipSlot()
{
	DefaultBorderColor = SlotBorder->GetBrushColor();

	/**set icon*/
	ItemIcon->SetIsEnabled(false);
	ItemIcon->SetToolTip(nullptr);
	ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	/**set border*/
	SlotBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SlotBorder->SetBrushColor(DefaultBorderColor);
}

void UCPP_EquipSlot::UpdateEquipSlot(class UItem* item)
{

}
void UCPP_EquipSlot::SetSlotToolTip(class UItem* item)
{
	if (IsValid(ToolTip))
	{
		/**if created tootip before, don't create widget and update that tootip*/
		ToolTip->SetTootipItemRef(item);
		ToolTip->UpdateToolTip();

		ItemIcon->SetToolTip(ToolTip);
	}
	else
	{
		/**CreateWidget only once*/
		if (TootipWidgetClass)
		{
			ToolTip = CreateWidget<UTootipWidget>(GetWorld(), TootipWidgetClass);
			ToolTip->SetTootipItemRef(item);
			ToolTip->UpdateToolTip();
			ItemIcon->SetToolTip(ToolTip);
		}
	}
}
