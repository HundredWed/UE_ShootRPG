// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CPP_Character.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Widget/CPP_Slot.h"
#include "Inventory.h"
#include "Widget/DragWidget.h"
#include "Widget/SetAmountWidget.h"

void UCPP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateWeightMaxAmount(InventoryRef->MaxWeight);
	UpdateWeightText(0);

	CloseButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::CloseWidget);
	SortButton->OnClicked.AddDynamic(this, &UCPP_InventoryWidget::SortInventory);
}

void UCPP_InventoryWidget::GenerateSlotWidget(const int16 slotsParRow)
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

					const int16 row = index / slotsParRow;
					const int16 column = index % slotsParRow;

					SlotPanel->AddChildToUniformGrid(SlotWidget, row, column);

					SlotWidget->UpdateSlot(index);
				}
			}
		}
	}
}

void UCPP_InventoryWidget::CloseWidget()
{
	PlayerRef->HideGameInventory();
	SpliteWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_InventoryWidget::SortInventory()
{
	InventoryRef->InventorySort(0, InventoryRef->SlotsArray.Num() - 1);
}

void UCPP_InventoryWidget::SetPanelEnabled(bool enabled)
{
	SlotPanel->SetIsEnabled(enabled);
}

void UCPP_InventoryWidget::SetSpliteWidget(const UCPP_Slot* fromSlot, const UCPP_Slot* toSlot)
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UUniformGridSlot not valid!!"))
		return;
	}
	
	FVector2D render = FVector2D(columnSize, clampRow);
	FWidgetTransform renderTransform = FWidgetTransform(render,FVector2D(1,1), FVector2D::Zero(), 0);

	SpliteWidget->SetRenderTransform(renderTransform);
	SpliteWidget->InitWidgetInfo(fromSlot->MyAmount, fromSlot->MyArrayNumber, false);
	SpliteWidget->ToIndex = toSlot->MyArrayNumber;
	SpliteWidget->SetVisibility(ESlateVisibility::Visible);
	SetPanelEnabled(false);
}

void UCPP_InventoryWidget::UpdateWeightText(const float amount)
{
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.MinimumFractionalDigits = 1;
	FText amountText = FText::AsNumber(amount, &NumberFormatOptions);
	WeightText->SetText(FText::Format(NSLOCTEXT("UCPP_InventoryWidget", "WeightText", "{0} / {1}"), amountText, MaxWeight));
}

void UCPP_InventoryWidget::UpdateWeightMaxAmount(const float amount)
{
	MaxWeight = amount;
}

void UCPP_InventoryWidget::UpdateGoldText(const int32 amount)
{
	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.UseGrouping = true;
	FText amountText = FText::AsNumber(amount, &NumberFormatOptions); 
	GoldText->SetText(FText::Format(NSLOCTEXT("UCPP_InventoryWidget", "GoldText", "{0}"), amountText));
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
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}



