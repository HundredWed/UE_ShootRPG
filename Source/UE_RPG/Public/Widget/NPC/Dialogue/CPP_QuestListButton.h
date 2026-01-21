// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/NPC/Dialogue/CPP_DialogueButtonBase.h"
#include "Structs/ST_Quest.h"
#include "CPP_QuestListButton.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_QuestListButton : public UCPP_DialogueButtonBase
{
	GENERATED_BODY()
	
public:

	void InitQuestListButton(const FQuest& questInfo);

protected:
	virtual void DialogueButtonEvent() override;

private:

	FQuest QuestInfo;
};
