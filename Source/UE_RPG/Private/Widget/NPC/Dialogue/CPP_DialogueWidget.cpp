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

void UCPP_DialogueWidget::NativeConstruct()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		DialogueSystem = GI->GetSubsystem<UCPP_DialogueSystem>();
		DialogueSystem->UpdateDialogueText.BindUObject(this, &UCPP_DialogueWidget::UpdateDialogueText);
		DialogueSystem->CreateAnswerBox.BindUObject(this, &UCPP_DialogueWidget::UpdateAnswerBox);
	}

	AnswerBox->SetVisibility(ESlateVisibility::Hidden);
	SetVisibilityQuestListBox(ESlateVisibility::Hidden);
}

void UCPP_DialogueWidget::InitDialogueWidget()
{
	if (IsValid(DialogueSystem))
	{
		FNPCDialogue npc = DialogueSystem->GetNPCStruct();

		NPCID = npc.NPCID;
		NPCName->SetText(npc.NPCName);
		DialogueText->SetText(npc.BasicDialogue);

		InitQuestSystem();
		SetInteractButton(npc);

		QuestListBox->UpdateQuestList();
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

	for (auto interactButton : InteractButtons)
	{
		if (interactButton.Value)
		{
			//퀘스트가 없는 npc면 퀘스트 버튼 생성x
			if (!HasAvailableQuest(npcInfo) && interactButton.Key == EInteractType::Quest)
				continue;

			UCPP_DialogueCategoryButton* button = CreateWidget<UCPP_DialogueCategoryButton>(GetWorld(), interactButton.Value);
			FMargin padding;
			padding.Left = 50.f;
			padding.Top = 30.f;
			padding.Right = 50.f;
			padding.Bottom = 0.f;

			button->SetPadding(padding);
			button->OnInteractButtonEvent.BindUObject(this, &UCPP_DialogueWidget::InteractButtonEvent);

			InteractContentsBox->AddChild(button);

			UHorizontalBoxSlot* slotButton = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(button);
			slotButton->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			slotButton->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);
		}
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

	TSubclassOf<UCPP_DialogueButtonBase> buttonClass = *(InteractButtons.Find(EInteractType::Revert));

	UCPP_DialogueCategoryButton* button = CreateWidget<UCPP_DialogueCategoryButton>(GetWorld(), buttonClass);
	FMargin padding;
	padding.Left = 50.f;
	padding.Top = 30.f;
	padding.Right = 50.f;
	padding.Bottom = 0.f;

	button->SetPadding(padding);
	button->OnInteractButtonEvent.BindUObject(this, &UCPP_DialogueWidget::InteractButtonEvent);

	DialogueSubBox->AddChild(button);

	UHorizontalBoxSlot* slotButton = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(button);
	slotButton->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	slotButton->SetVerticalAlignment(EVerticalAlignment::VAlign_Top);


	USpacer* backSpace = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	DialogueSubBox->AddChild(backSpace);
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
		SetVisibilityQuestListBox(ESlateVisibility::Visible);
		break;
	case EInteractType::LikeAbility:
		break;
	case EInteractType::Revert:
		ActivateDialogueSubBox(ESlateVisibility::Hidden);
		InitDialogueWidget();
		ActivateDialogueMainBox(ESlateVisibility::Visible);
		break;
	case EInteractType::Quit:
		this->SetVisibility(ESlateVisibility::Hidden);
		break;
	default:
		break;
	}
}

void UCPP_DialogueWidget::InitQuestSystem()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* quest = GI->GetSubsystem<UCPP_QuestSubsystem>();
		quest->InitQuestSubsystem(NPCID);
	}
}

void UCPP_DialogueWidget::AnswerEvent()
{
	AnswerBox->SetVisibility(ESlateVisibility::Hidden);
}

bool UCPP_DialogueWidget::HasAvailableQuest(const FNPCDialogue& npcInfo)
{
	bool canQuest = npcInfo.CanQuest;
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


	return canQuest || hasQuest;
}

void UCPP_DialogueWidget::UpdateDialogueText(const FText& text)
{
	DialogueText->SetText(text);
}

void UCPP_DialogueWidget::UpdateAnswerBox(TArray<FAnswerDialogue> answers)
{
	for (FAnswerDialogue anwser : answers)
	{
		UCPP_AnswerButton* button = CreateWidget<UCPP_AnswerButton>(GetWorld(), AnswerButtonClass);
		button->InitAnswerButton(anwser);
		button->OnAnswerEvent.BindUObject(this, &UCPP_DialogueWidget::AnswerEvent);

		AnswerBox->AddAnswerList(button);
	}
}

void UCPP_DialogueWidget::SetVisibilityQuestListBox(ESlateVisibility visibility)
{
	QuestListBox->SetVisibility(visibility);
}

