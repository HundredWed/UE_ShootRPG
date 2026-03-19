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
#include "Widget/Player/Quest/CPP_InProgressQuestsWidget.h"

void UMainPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainPanelWidget::BindCharacterStat(ACPP_Character* player)
{
	player->OnUpdatePlayerState.BindUObject(this, &UMainPanelWidget::InitState);
	player->OnUpdateHP.BindUObject(this, &UMainPanelWidget::UpdateHealthBarPercent);
}

void UMainPanelWidget::InitState(const FCharacterStats& stat)
{
	if (!IsValid(StateWidget))
		return;

	StateWidget->InitStateBar(stat);
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

bool UMainPanelWidget::ToggleQuestList()
{
	if (IsQuestListActivate)
	{
		InProgressQuestsWidget->SetCustomVisibility(ESlateVisibility::Hidden);
		IsQuestListActivate = false;
	}
	else
	{
		InProgressQuestsWidget->SetCustomVisibility(ESlateVisibility::Visible);
		IsQuestListActivate = true;
	}	

	return IsQuestListActivate;
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
				bool successRemove = InventoryRef->RemoveItemAtIndex(slotIndex, amount);
				if (successRemove == false)
				{
					//TODO
					//버리지 못하는 안내창 출력
					UpdatePopupText(FText::FromString(TEXT("해당 아이템은 버릴 수 없습니다.")));
					PopupWidget->SetVisibility(ESlateVisibility::Visible);
				}
				return successRemove;
			}

		}
	}
	
	return true;
}
