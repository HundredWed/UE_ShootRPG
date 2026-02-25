// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ST_Quest.h"
#include "CPP_QuestListBox.generated.h"

class UScrollBox;
class UCPP_QuestListButton;

UCLASS()
class UE_RPG_API UCPP_QuestListBox : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateQuestList();

private:

	UFUNCTION()
	void SelectedQuest(const FQuest& quest);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UScrollBox* QuestList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCPP_QuestListButton> QuestButtonClass;

};
