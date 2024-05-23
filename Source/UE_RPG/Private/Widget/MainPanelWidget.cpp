// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainPanelWidget.h"

#include "CPP_Character.h"
#include "Inventory.h"
#include "Widget/CPP_InventoryWidget.h"
#include "Widget/SetAmountWidget.h"
#include "Widget/DragWidget.h"
#include "Widget/SlotDrag.h"
#include "Widget/CPP_Slot.h"
#include "Widget/CPP_PlayerStateBar.h"

void UMainPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(PlayerRef))
	{
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->GenerateSlotWidget(PlayerRef->GetInventoryRowSize());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("not Found InventoryWidget at MainPanel!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not Found PlayerRef at MainPanel!!"));
	}
}

void UMainPanelWidget::InitState(const int32 level, const float healt, const float maxHP, const float mana, const float stamina)
{
	if (!IsValid(StateWidget))
		return;

	StateWidget->InitStateBar(level, healt, maxHP, mana, stamina);
}

void UMainPanelWidget::UpdateHealthBarPercent(const float currentHp, const float max)
{
	StateWidget->UpdateHealthBarPercent(currentHp, max);
}

void UMainPanelWidget::UpdateManaBarPercent(const float percent)
{
	StateWidget->UpdateManaBarPercent(percent);
}

void UMainPanelWidget::UpdateLevel(int32 level)
{
	StateWidget->UpdateLevel(level);
}

bool UMainPanelWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	
	UDragWidget* inDargWidget = Cast<UDragWidget>(InOperation);/**inventory DragWidget*/
	if (IsValid(inDargWidget))
	{

		FVector2D dropPos = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		FVector2D dragPos = inDargWidget->DragOffset;

		FVector2D viewPortPos = dropPos - dragPos;

		inDargWidget->WidgetRef->AddToViewport();
		inDargWidget->WidgetRef->SetVisibility(ESlateVisibility::Visible);
		inDargWidget->WidgetRef->SetPositionInViewport(viewPortPos,false);

	}
	else
	{
		USlotDrag* inSlotDragWidget = Cast<USlotDrag>(InOperation);
		if (inSlotDragWidget)
		{
			const uint8 slotIndex = inSlotDragWidget->WidgetRef->MyArrayNumber;
			const int32 amount = inSlotDragWidget->WidgetRef->MyAmount;
			bool bstackable = inSlotDragWidget->WidgetRef->bMyItemCanStacked;

			if ((amount > 1) && bstackable)
			{
				ThrowWidget->InitWidgetInfo(amount, slotIndex, true);
				ThrowWidget->SetVisibility(ESlateVisibility::Visible);
				InventoryWidget->SetPanelEnabled(false);
				true;
			}
			else
			{
				InventoryRef->RemoveItemAtIndex(slotIndex, amount);
				return true;
			}

		}
	}
	
	return true;
}
