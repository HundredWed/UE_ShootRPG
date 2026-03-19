// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/NPC/Dialogue/CPP_DialogueButtonBase.h"
#include "Structs/ST_DialogueAnswer.h"
#include "CPP_AnswerButton.generated.h"

struct FAnswerInfo
{
	FName ReturnRowName;
	EAnswerType AnswerType = EAnswerType::Normal;
	bool bQuestAnswer;
};


DECLARE_DELEGATE_OneParam(FOnAnswerEventDelegate, const FAnswerInfo&);

UCLASS()
class UE_RPG_API UCPP_AnswerButton : public UCPP_DialogueButtonBase
{
	GENERATED_BODY()
	
public:
	void InitAnswerButton(const FAnswerDialogue& answer, bool questAnswer = false);

	FOnAnswerEventDelegate OnAnswerEvent;

protected:
	virtual void DialogueButtonEvent() override;

private:

	FAnswerInfo AnswerInfo;
};
