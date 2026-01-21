// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_QuestListBox.h"
#include "Components/ScrollBox.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Widget/NPC/Dialogue/CPP_QuestListButton.h"

void UCPP_QuestListBox::UpdateQuestList()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* QS = GI->GetSubsystem<UCPP_QuestSubsystem>();
		TArray<FQuest> quests = QS->GetQuestList();

		for (FQuest quest : quests)
		{
			UCPP_QuestListButton* list = CreateWidget<UCPP_QuestListButton>(GetWorld(), QuestButtonClass);
			//list->init();
			
			list->AddToViewport();
			QuestList->AddChild(list);
		}
		
	}
}