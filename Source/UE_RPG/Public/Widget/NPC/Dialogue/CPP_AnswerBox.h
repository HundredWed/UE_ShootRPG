// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_AnswerBox.generated.h"

class UVerticalBox;

UCLASS()
class UE_RPG_API UCPP_AnswerBox : public UUserWidget
{
	GENERATED_BODY()
public:

	void AddAnswerList(UUserWidget* answser);


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UVerticalBox* AnswerListBox;
};
