// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/NPC/Dialogue/CPP_DialogueButtonBase.h"
#include "Structs/ST_Quest.h"
#include "CPP_QuestListButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnButtonClicked, const FQuest&);


UCLASS()
class UE_RPG_API UCPP_QuestListButton : public UCPP_DialogueButtonBase
{
	GENERATED_BODY()
	
public:

	void InitQuestListButton(const FQuest& questInfo, bool bShowProgress = false);

	FOnButtonClicked OnButtonClicked;

protected:
	virtual void DialogueButtonEvent() override;

private:

	FText SetFormatText();

	FQuest QuestInfo;
};
