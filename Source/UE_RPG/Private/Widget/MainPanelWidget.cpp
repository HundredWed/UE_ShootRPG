// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainPanelWidget.h"

#include "CPP_Character.h"
#include "Component/CPP_StatComponent.h"
#include "Inventory.h"
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
}

void UMainPanelWidget::BindCharacterStat(UCPP_StatComponent* statComponent)
{
	statComponent->OnUpdateCharacterState.BindUObject(this, &UMainPanelWidget::InitState);
	statComponent->OnUpdateHP.AddUObject(this, &UMainPanelWidget::UpdateHealthBarPercent);

	ACPP_Controller* PC = Cast<ACPP_Controller>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PC->OnScreenBlackEvent.AddUObject(this, &UMainPanelWidget::PlayFadeIn);
	PC->OnScreenRevealEvent.AddUObject(this, &UMainPanelWidget::PlayFadeOut);

	//플레이어 생성시 한번만 콜되니 여기서 초기화
	InitState(FCharacterStats());
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
		if (inSlotDragWidget)
		{
			const uint8 slotIndex = inSlotDragWidget->WidgetRef->MyIndex;
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
				//임시 로직
				// ui매니저에 위젯을 등록하는 방법을 바꾸면서 리팩토링
				//===================================================================================
				ACPP_Character* player = Cast<ACPP_Character>(GetOwningPlayerPawn());
				bool successRemove = player->GetInventory()->RemoveItemAtIndex(slotIndex, amount);
				if (successRemove == false)
				{
					//TODO
					//버리지 못하는 안내창 출력
					UpdatePopupText(FText::FromString(TEXT("해당 아이템은 버릴 수 없습니다.")));
					PopupWidget->SetVisibility(ESlateVisibility::Visible);
				}
				//===================================================================================
				return successRemove;
			}

		}
	}
	
	return true;
}
