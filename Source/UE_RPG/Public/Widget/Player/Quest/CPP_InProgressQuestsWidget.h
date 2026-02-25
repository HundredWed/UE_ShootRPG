// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "Structs/ST_Quest.h"
#include "CPP_InProgressQuestsWidget.generated.h"


class UScrollBox;
class UTextBlock;
class UCPP_QuestListButton;

UCLASS()
class UE_RPG_API UCPP_InProgressQuestsWidget : public UCustomUMGWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void InitQuestList();
	void AddQuestList(const FQuest& quest);
	void RemoveQuestList(const FName& questID);

	void SetCustomVisibility(ESlateVisibility visibility);

private:

	UFUNCTION()
	void UpdateQuestText(const FQuest& quest);
	UFUNCTION()
	void UpdateQuestObjective(const FQuest& quest);
	UFUNCTION()
	void UpdateQuestInfo(const FQuest& quest);
	UFUNCTION()
	void OnHiddenAnimationFinished();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UScrollBox* QuestListBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* QuestDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* QuestObjective;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TSubclassOf<UCPP_QuestListButton> QuestButtonListClass;

	UPROPERTY(Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* VisibleAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* HiddenAnimation;

	TMap<FName, TObjectPtr<UCPP_QuestListButton>> AllChildrenOfListBox;
};
