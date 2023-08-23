// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Widget/CPP_Slot.h"
#include "Inventory.h"

void UCPP_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCPP_InventoryWidget::GenerateSlotWidget(int32 slotsParRow)
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
					//SlotWidget->SetInventory(InventoryRef);
					SlotWidgetArray.Add(SlotWidget);

					int32 row = index / slotsParRow;
					int32 column = index % slotsParRow;

					SlotPanel->AddChildToUniformGrid(SlotWidget, row, column);

					SlotWidget->UpdateSlot(index);
				}
			}

		}
	}
	

}
