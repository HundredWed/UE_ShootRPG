// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_DialogueWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Spacer.h"
#include "Blueprint/WidgetTree.h"

#include "Widget/NPC/Dialogue/CPP_DialogueCategoryButton.h"
#include "Widget/NPC/Dialogue/CPP_AnswerButton.h"
#include "Widget/NPC/Dialogue/CPP_AnswerBox.h"
#include "Widget/NPC/Dialogue/CPP_QuestListBox.h"
#include "Systems/CPP_DialogueSystem.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Inventory.h"

void UCPP_DialogueWidget::NativeConstruct()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		DialogueSystem = GI->GetSubsystem<UCPP_DialogueSystem>();
		DialogueSystem->UpdateDialogueText.BindUObject(this, &UCPP_DialogueWidget::UpdateDialogueEvent);
		DialogueSystem->OnUpdateAnswerBox.BindUObject(this, &UCPP_DialogueWidget::UpdateAnswerBox);
		DialogueSystem->EndDialogue.BindUObject(this, &UCPP_DialogueWidget::RevertEvent);

		QuestSubsystem = GI->GetSubsystem<UCPP_QuestSubsystem>();

		check(DialogueSystem);
		check(QuestSubsystem);
	}

	CreateAnswerButton();
}

void UCPP_DialogueWidget::InitDialogueWidget()
{
	if (IsValid(DialogueSystem))
	{
		FNPCDialogue npc = DialogueSystem->GetNPCStruct();

		NPCName->SetText(npc.NPCName);
		DialogueText->SetText(npc.BasicDialogue);

		SetInteractButton(npc);
		
		QuestListBox->InitQuestListBox();
		SetVisibilityQuestListBox(ESlateVisibility::Hidden);
	}	
}

void UCPP_DialogueWidget::ActivateAnswerBox(bool bActivate)
{
	if (bActivate)
	{
		AnswerBox->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AnswerBox->SetVisibility(ESlateVisibility::Hidden);
	}	
}

void UCPP_DialogueWidget::CreateInteractButton()
{
	
}

void UCPP_DialogueWidget::ResetToInitialState()
{
	SetVisibilityQuestListBox(ESlateVisibility::Hidden);
	FNPCDialogue npc = DialogueSystem->GetNPCStruct();
	DialogueText->SetText(npc.BasicDialogue);
	SetInteractButton(npc);
	SetAllButtonToCollapsed();	
}

void UCPP_DialogueWidget::SetInteractButton(const FNPCDialogue& npcInfo)
{
	SetMainBox(npcInfo);
	SetSubBox(npcInfo);
}

void UCPP_DialogueWidget::SetMainBox(const FNPCDialogue& npcInfo)
{
	InteractContentsBox->ClearChildren();

	TArray<UWidget*> allChildren;

	USpacer* forwardSpace = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	InteractContentsBox->AddChild(forwardSpace);

	UHorizontalBoxSlot* forwardSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(forwardSpace);
	FSlateChildSize size;
	forwardSlot->SetSize(size);
	


	for (EInteractType interactType : npcInfo.CanInteractTypes)
	{
		//퀘스트가 없는 npc면 퀘스트 버튼 생성x
		//Revert는 메인에 생성x
		if ((!HasAvailableQuest(npcInfo) && interactType == EInteractType::Quest) 
			|| interactType == EInteractType::Revert)
			continue;

		UCPP_DialogueCategoryButton* button = CreateWidget<UCPP_DialogueCategoryButton>(GetWorld(), InteractButtonsClass);
		FMargin padding;
		padding.Left = 50.f;
		padding.Top = 30.f;
		padding.Right = 50.f;
		padding.Bottom = 0.f;

		button->SetPadding(padding);
		button->InitType(interactType);
		button->OnInteractButtonEvent.BindUObject(this, &UCPP_DialogueWidget::InteractButtonEvent);

		InteractContentsBox->AddChild(button);

		UHorizontalBoxSlot* slotButton = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(button);
		slotButton->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
		slotButton->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
	}


	USpacer* backSpace = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	InteractContentsBox->AddChild(backSpace);

	UHorizontalBoxSlot* backSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(backSpace);
	backSlot->SetSize(size);
}

void UCPP_DialogueWidget::SetSubBox(const FNPCDialogue& npcInfo)
{
	DialogueSubBox->ClearChildren();

	TArray<UWidget*> allChildren;
	USpacer* forwardSpace = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	DialogueSubBox->AddChild(forwardSpace);

	UHorizontalBoxSlot* forwardSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(forwardSpace);
	FSlateChildSize size;
	forwardSlot->SetSize(size);

	UCPP_DialogueCategoryButton* button = CreateWidget<UCPP_DialogueCategoryButton>(GetWorld(), InteractButtonsClass);
	FMargin padding;
	padding.Left = 50.f;
	padding.Top = 30.f;
	padding.Right = 50.f;
	padding.Bottom = 0.f;

	button->SetPadding(padding);
	button->InitType(EInteractType::Revert);
	button->OnInteractButtonEvent.BindUObject(this, &UCPP_DialogueWidget::InteractButtonEvent);

	DialogueSubBox->AddChild(button);

	UHorizontalBoxSlot* slotButton = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(button);
	slotButton->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	slotButton->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);


	USpacer* backSpace = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	DialogueSubBox->AddChild(backSpace);

	UHorizontalBoxSlot* backSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(backSpace);
	backSlot->SetSize(size);
}

void UCPP_DialogueWidget::InteractButtonEvent(EInteractType interactType)
{
	ActivateDialogueSubBox(ESlateVisibility::Visible);
	ActivateDialogueMainBox(ESlateVisibility::Hidden);


	switch (interactType)
	{
	case EInteractType::Normal:
		break;
	case EInteractType::JustTalk:
		break;
	case EInteractType::Quest:
		QuestListBox->UpdateQuestList();
		SetVisibilityQuestListBox(ESlateVisibility::Visible);
		break;
	case EInteractType::LikeAbility:
		break;
	case EInteractType::Revert:
		RevertEvent();
		break;
	case EInteractType::Quit:
		ActivateDialogueMainBox(ESlateVisibility::Visible);
		ActivateDialogueSubBox(ESlateVisibility::Hidden);
		this->SetVisibility(ESlateVisibility::Hidden);
		break;
	default:
		break;
	}
}

void UCPP_DialogueWidget::AnswerEvent(const FAnswerInfo& answerInfo)
{
	AnswerBox->SetVisibility(ESlateVisibility::Hidden);
	DialogueSystem->SelectedAnswer(answerInfo.ReturnRowName);


	if (answerInfo.bQuestAnswer && answerInfo.AnswerType == EAnswerType::Positive)
	{
		QuestSubsystem->AddProgressQuest();

		UInventory* inventory = GetOwningPlayerPawn()->FindComponentByClass<UInventory>();
		if (inventory)
		{
			int32 amount = inventory->GetTotalItemAmount(QuestSubsystem->GetCurrentQuest().NeedContentID);
			QuestSubsystem->CheckQuestContent(QuestSubsystem->GetCurrentQuest().NeedContentID, amount);
		}		
	}
}

void UCPP_DialogueWidget::CreateAnswerButton()
{
	for (int32 i = 0; i < AnswerButtonCount; i++)
	{
		UCPP_AnswerButton* button = CreateWidget<UCPP_AnswerButton>(GetWorld(), AnswerButtonClass);
		button->OnAnswerEvent.BindUObject(this, &UCPP_DialogueWidget::AnswerEvent);

		button->SetVisibility(ESlateVisibility::Collapsed);
		AnswerBox->AddAnswerList(button);
		AnswerButtonStorage.Add(button);
	}

	AnswerBox->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_DialogueWidget::SetAllButtonToCollapsed()
{
	//대답버튼
	for (UUserWidget* wd : AnswerButtonStorage)
	{
		wd->SetVisibility(ESlateVisibility::Collapsed);
	}

	//퀘스트 리스트
	QuestListBox->SetButtonToCollapsed();
}

bool UCPP_DialogueWidget::HasAvailableQuest(const FNPCDialogue& npcInfo)
{
	bool hasQuest = false;

	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* quest = GI->GetSubsystem<UCPP_QuestSubsystem>();

		if (quest)
		{
			hasQuest = quest->GetQuestList().IsEmpty();
		}
	}


	return !hasQuest;
}

void UCPP_DialogueWidget::UpdateDialogueEvent(const FText& text, EDialogueEventType dialogueEventType, const FName& eventId)
{
	
	DialogueText->SetText(text);

	switch (dialogueEventType)
	{
	case EDialogueEventType::None:		
		break;
	case EDialogueEventType::GiveQuestReward:
		//보류
		//RewardBox->ActivateRewardBox();
		QuestSubsystem->QuestClear();
		break;
	case EDialogueEventType::ShowSpecificUI:
		break;
	case EDialogueEventType::TriggerCinematic:
		break;
	case EDialogueEventType::TriggerGimmick:
		DialogueSystem->OnTriggeredGimmickEvent(eventId);
		break;
	default:
		break;
	}	
}

void UCPP_DialogueWidget::UpdateAnswerBox(TArray<FAnswerDialogue> answers, bool questAnswer)
{
	int32 amount = 0;

	for (FAnswerDialogue answer : answers)
	{
		AnswerButtonStorage[amount]->InitAnswerButton(answer, questAnswer);
		AnswerButtonStorage[amount]->SetVisibility(ESlateVisibility::Visible);
		amount++;
	}

	ActivateAnswerBox(true);
}

void UCPP_DialogueWidget::SetVisibilityQuestListBox(ESlateVisibility visibility)
{
	QuestListBox->SetVisibility(visibility);
}

void UCPP_DialogueWidget::RevertEvent()
{
	ActivateDialogueSubBox(ESlateVisibility::Hidden);
	ResetToInitialState();
	ActivateDialogueMainBox(ESlateVisibility::Visible);
}

