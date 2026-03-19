// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_QuestListBox.h"
#include "Components/ScrollBox.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Systems/CPP_DialogueSystem.h"
#include "Widget/NPC/Dialogue/CPP_QuestListButton.h"


void UCPP_QuestListBox::InitQuestListBox()
{
	for (int32 i = 0; i < QuestButtonCount; i++)
	{
		UCPP_QuestListButton* list = CreateWidget<UCPP_QuestListButton>(GetWorld(), QuestButtonClass);
		list->OnButtonClicked.BindUObject(this, &UCPP_QuestListBox::SelectedQuest);

		list->SetVisibility(ESlateVisibility::Collapsed);
		QuestList->AddChild(list);
		QuestListButtonStorage.Add(list);
	}
}

void UCPP_QuestListBox::UpdateQuestList()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* QS = GI->GetSubsystem<UCPP_QuestSubsystem>();
		TArray<FQuest> quests = QS->GetQuestList();

		int32 amount = 0;
		for (FQuest quest : quests)
		{
			QuestListButtonStorage[amount]->InitQuestListButton(quest, true);
			QuestListButtonStorage[amount]->SetVisibility(ESlateVisibility::Visible);
			amount++;
		}		
	}
}

void UCPP_QuestListBox::SetButtonToCollapsed()
{
	for (UUserWidget* wd : QuestListButtonStorage)
	{
		wd->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCPP_QuestListBox::SelectedQuest(const FQuest& quest)
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* QS = GI->GetSubsystem<UCPP_QuestSubsystem>();
		QS->SelectedQuest(quest.QuestID);

		UCPP_DialogueSystem* DS = GI->GetSubsystem<UCPP_DialogueSystem>();
		DS->SelectedQuest(quest);
	}

	SetVisibility(ESlateVisibility::Hidden);
}
