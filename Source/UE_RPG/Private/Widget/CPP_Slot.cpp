// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Widget/SetAmountWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Widget/TootipWidget.h"
#include "Widget/CPP_EquipmentInventory.h"
#include "Widget/CPP_EquipSlot.h"

void UCPP_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	CombineButton->OnPressed.AddDynamic(this, &UCPP_Slot::CombineItem);
	DefaultBorderColor = SlotBorder->GetBrushColor();
}

void UCPP_Slot::UpdateSlot(const FItemInfoTable* itemData, const int32 index,  const int32 amount)
{
	if (itemData)
	{
		MyIndex = index;
		MyAmount = amount;
		bMyItemCanStacked = itemData->bCanStacked;
		ActiveSlot(itemData->IconTexture);

		if (itemData->bCanStacked && MyAmount > 0)
		{
			TextAmount->SetText(FText::Format(NSLOCTEXT("CPP_Slot", "TextAmount", "x{0}"), MyAmount));
			TextAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			TextAmount->SetVisibility(ESlateVisibility::Hidden);
		}

		/**set tooltip*/
		SetSlotToolTip(itemData);
		SetBorderWhite();
	}
}

void UCPP_Slot::UpdateSlot(const FItemInfoTable* itemData, const FEquipmentInfoTable* equipmentData, const int32 index)
{
	if (itemData)
	{
		MyIndex = index;
		MyAmount = 1;
		bMyItemCanStacked = itemData->bCanStacked;
		ActiveSlot(itemData->IconTexture);
		TextAmount->SetVisibility(ESlateVisibility::Hidden);
	
		/**set tooltip*/
		SetSlotToolTip(itemData);
		SetBorderWhite();
	}
}


void UCPP_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	OnSlotDragDetected.Execute(InGeometry, InMouseEvent, OutOperation, MyIndex);
}

bool UCPP_Slot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

void UCPP_Slot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool UCPP_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return OnSlotDrop.Execute(InGeometry, InDragDropEvent, InOperation,MyIndex);
}

FReply UCPP_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (ItemIcon->GetIsEnabled())
	{
		OnSlotMouseButtonDown.Execute(InGeometry, InMouseEvent, MyIndex);
		FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return reply.NativeReply;
	}

	return FReply::Handled();
}

FReply UCPP_Slot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		//icon
		if (ItemIcon->GetIsEnabled())
		{
			return OnSlotMouseButtonDoubleClick.Execute(InGeometry, InMouseEvent, MyIndex);
		}
	}
	return FReply::Handled();
}

void UCPP_Slot::InactiveSlot()
{
	Super::InactiveSlot();

	/**set Text*/
	TextAmount->SetVisibility(ESlateVisibility::Hidden);
	/**set CombineButton*/
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_Slot::ActiveSlot(UTexture2D* icon)
{
	Super::ActiveSlot(icon);
}

void UCPP_Slot::InitSlotInfo(const int32 index)
{
	MyIndex = index;
	MyAmount = 0;
	bMyItemCanStacked = false;
	InactiveSlot();
}

void UCPP_Slot::InactiveCombinableSlot()
{
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
	bActiveCombineButton = false;
}

void UCPP_Slot::ActiveCombinableSlot()
{
	CombineButton->SetVisibility(ESlateVisibility::Visible);
	bActiveCombineButton = true;
}

void UCPP_Slot::CombineItem()
{
	OnCombineDelegate.Execute(MyIndex);
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
	bActiveCombineButton = false;
}

void UCPP_Slot::SetBorder()
{
	SlotBorder->SetBrushColor(DefaultBorderColor);
}

