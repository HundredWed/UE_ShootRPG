// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Widget/CPP_Slot.h"
#include "Inventory.h"
#include "Widget/DragWidget.h"
#include "Widget/SetAmountWidget.h"

void UCPP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::CloseWidget);
	
}

void UCPP_InventoryWidget::GenerateSlotWidget(const uint8 slotsParRow)
{
	if (IsValid(InventoryRef))
	{
		SlotPanel->ClearChildren();

		SlotWidgetArray.Empty();

		for (int32 index = 0; index < InventoryRef->SlotsArray.Num(); index++)
		{
			if (SlotWidgetClass)
			{
				SlotWidget = CreateWidget<UCPP_Slot>(this, SlotWidgetClass);
				if (IsValid(SlotWidget))
				{
					SlotWidgetArray.Add(SlotWidget);

					const uint8 row = index / slotsParRow;
					const uint8 column = index % slotsParRow;

					SlotPanel->AddChildToUniformGrid(SlotWidget, row, column);

					SlotWidget->UpdateSlot(index);
				}
			}
		}
	}
}

void UCPP_InventoryWidget::CloseWidget()
{
	InventoryRef->HideInventory();
}

void UCPP_InventoryWidget::SetPanelEnabled(bool enabled)
{
	SlotPanel->SetIsEnabled(enabled);
}

void UCPP_InventoryWidget::SetSpliteWidget(UCPP_Slot* fromSlot, UCPP_Slot* toSlot)
{
	UUniformGridSlot* slotGrid = Cast<UUniformGridSlot>(toSlot->Slot);
	float rowSize;
	float clampRow;
	float columnSize;
	if (IsValid(slotGrid))
	{
		rowSize = (float)(slotGrid->GetRow() * SlotBoxSize) - (InventoryScollBox->GetScrollOffset());
		clampRow = FMath::Clamp(rowSize, 0, InventoryBoxSize);
		columnSize = (float)(slotGrid->GetColumn() * SlotBoxSize);
	}
	
	FVector2D render = FVector2D(columnSize, clampRow);
	FWidgetTransform renderTransform = FWidgetTransform(render,FVector2D(1,1), FVector2D::Zero(), 0);

	SpliteWidget->SetRenderTransform(renderTransform);
	SpliteWidget->InitWidgetInfo(fromSlot->MyAmount, fromSlot->MyArrayNumber, false);
	SpliteWidget->ToIndex = toSlot->MyArrayNumber;
	SpliteWidget->SetVisibility(ESlateVisibility::Visible);
	SetPanelEnabled(false);
}

FReply UCPP_InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this,EKeys::LeftMouseButton);

	return reply.NativeReply;
}

void UCPP_InventoryWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	UDragWidget* dragWidget = Cast<UDragWidget>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragWidget::StaticClass()));

	this->SetVisibility(ESlateVisibility::HitTestInvisible);

	if (IsValid(dragWidget))
	{
		dragWidget->WidgetRef = this;
		dragWidget->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		dragWidget->DefaultDragVisual = this;
		dragWidget->Pivot = EDragPivot::MouseDown;
	}
	
	OutOperation = dragWidget;
}

void UCPP_InventoryWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	UDragWidget* dragWidget = Cast<UDragWidget>(InOperation);
	if (dragWidget)
	{
		RemoveFromParent();
	}
}

bool UCPP_InventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}



