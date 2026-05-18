// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainPanelWidget.h"

#include "CPP_Character.h"
#include "Component/CPP_StatComponent.h"
#include "Widget/CPP_InventoryWidget.h"
#include "Widget/SetAmountWidget.h"
#include "Widget/DragWidget.h"
#include "Widget/SlotDrag.h"
#include "Widget/CPP_Slot.h"
#include "Widget/CPP_PlayerStateBar.h"
#include "Widget/Player/Quest/CPP_InProgressQuestsWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Controller.h"

void UMainPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideInteractWidget();

	ThrowWidget->OnCancelEvent.BindUObject(this, &UMainPanelWidget::ClosedThrowWidget);
	ThrowWidget->OnThrowEvent.BindUObject(this, &UMainPanelWidget::ThrowItem);
}

void UMainPanelWidget::ShowInteractWidget()
{
	InteractWidget->SetVisibility(ESlateVisibility::Visible);
}

void UMainPanelWidget::HideInteractWidget()
{
	InteractWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UMainPanelWidget::BindCharacter(ACPP_Character* player)
{
	player->OnLookAtTalker.AddUObject(this, &UMainPanelWidget::ShowInteractWidget);
	player->OnEndLookAtTalker.AddUObject(this, &UMainPanelWidget::HideInteractWidget);

	player->GetStatComponent()->OnUpdateCharacterState.BindUObject(this, &UMainPanelWidget::InitState);
	player->GetStatComponent()->OnUpdateHP.AddUObject(this, &UMainPanelWidget::UpdateHealthBarPercent);
	player->GetStatComponent()->OnUpdateMP.AddUObject(this, &UMainPanelWidget::UpdateManaBarPercent);
	player->GetStatComponent()->OnOverHeat.AddUObject(this, &UMainPanelWidget::ManaOverHeatEvent);
	player->GetStatComponent()->OnCoolDown.AddUObject(this, &UMainPanelWidget::ManaCoolDownEvent);
	player->GetStatComponent()->OnRecoverMP.AddUObject(this, &UMainPanelWidget::RecoverManaBar);

	ACPP_Controller* PC = Cast<ACPP_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PC->OnScreenBlackEvent.AddUObject(this, &UMainPanelWidget::PlayFadeIn);
	PC->OnScreenRevealEvent.AddUObject(this, &UMainPanelWidget::PlayFadeOut);

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

void UMainPanelWidget::UpdateManaBarPercent(const float currentMP, const float max)
{
	const float percent = currentMP / max;
	StateWidget->UpdateManaBarPercent(percent);
}

void UMainPanelWidget::RecoverManaBar(const float percent)
{
	StateWidget->StartManaRecover(percent);
}

void UMainPanelWidget::ManaOverHeatEvent()
{
	ManaOverHeat();
}

void UMainPanelWidget::ManaCoolDownEvent()
{
	ManaCoolDown();
}

void UMainPanelWidget::ThrowItem(const int32 index, const int32 amount)
{
	InventoryWidget->RequestThrowItem(index, amount);
	InventoryWidget->SetPanelEnabled(true);
}

void UMainPanelWidget::ClosedThrowWidget()
{
	InventoryWidget->SetPanelEnabled(true);
}

void UMainPanelWidget::UpdateLevel(int32 level)
{
	StateWidget->UpdateLevel(level);
}

void UMainPanelWidget::PlayFadeIn()
{
	if (FadeAnim)
	{
		PlayAnimation(FadeAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		IsPrevBackScreenEvent = true;
	}
}

void UMainPanelWidget::PlayFadeOut()
{
	if (FadeAnim && IsPrevBackScreenEvent)
	{
		PlayAnimation(FadeAnim, 0.0f, 1, EUMGSequencePlayMode::Reverse, 1.0f);
		IsPrevBackScreenEvent = false;
	}
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
		if (!inSlotDragWidget)
		{
			return false;
		}

		const uint8 slotIndex = inSlotDragWidget->SlotIndex;
		UCPP_Slot* slot = InventoryWidget->GetSlotWidget(slotIndex);

		if (!IsValid(slot))
		{
			return false;
		}

		const int32 amount = slot->MyAmount;
		bool bStackable = slot->bMyItemCanStacked;

		if ((amount > 1) && bStackable)
		{
			ThrowWidget->InitWidgetInfo(amount, slotIndex, true);
			ThrowWidget->SetVisibility(ESlateVisibility::Visible);
			InventoryWidget->SetPanelEnabled(false);
		}
		else
		{
			bool successRemove = InventoryWidget->RequestThrowItem(slotIndex, amount);
			if (!successRemove)
			{
				UpdatePopupText(FText::FromString(TEXT("해당 아이템은 버릴 수 없습니다.")));
				PopupWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}	
	}
	
	return true;
}
