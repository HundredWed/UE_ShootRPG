// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPP_AnswerButton.h"
#include "Structs/ST_NPC.h"
#include "Structs/ST_Quest.h"
#include "CPP_DialogueWidget.generated.h"

class UCPP_QuestListBox;
class UCPP_AnswerBox;
class UTextBlock;
class UHorizontalBox;
class UCPP_DialogueButtonBase;
class UCPP_AnswerButton;
class UCPP_AnswerBox;
class UCPP_DialogueSystem;
class UCPP_QuestSubsystem;
class UCPP_QuestRewardBox;

UCLASS()
class UE_RPG_API UCPP_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void InitDialogueWidget();
	
	UFUNCTION()
	void UpdateDialogueEvent(const FText& text, EDialogueEventType dialogueEventType);
	UFUNCTION()
	void UpdateAnswerBox(TArray<FAnswerDialogue> answers, bool questAnswer = false);

	UFUNCTION(BlueprintImplementableEvent)
	bool ActivateDialogueSubBox(ESlateVisibility Invisibility);

	UFUNCTION(BlueprintImplementableEvent)
	bool ActivateDialogueMainBox(ESlateVisibility Invisibility);

private:

	void CreateInteractButton();

	void ResetToInitialState();

	void SetInteractButton(const FNPCDialogue& npcInfo);
	void SetMainBox(const FNPCDialogue& npcInfo);
	void SetSubBox(const FNPCDialogue& npcInfo);

	UFUNCTION()
	void InteractButtonEvent(EInteractType interactType);

	void ActivateAnswerBox(bool bActivate);
	void AnswerEvent(const FAnswerInfo& answerInfo);
	void CreateAnswerButton();
	void SetAllButtonToCollapsed();

	void SetVisibilityQuestListBox(ESlateVisibility visibility);

	UFUNCTION()
	void RevertEvent();

	bool HasAvailableQuest(const FNPCDialogue& npcInfo);


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* InteractContentsBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* DialogueSubBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_QuestListBox* QuestListBox;
	
	//보류
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_QuestRewardBox* QuestRewardBox;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_AnswerBox* AnswerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* NPCName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* DialogueText;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UCPP_DialogueButtonBase> InteractButtonsClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UCPP_AnswerButton> AnswerButtonClass;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	int32 AnswerButtonCount = 3;

	UPROPERTY()
	TObjectPtr<UCPP_DialogueSystem> DialogueSystem;

	UPROPERTY()
	TObjectPtr<UCPP_QuestSubsystem> QuestSubsystem;

	UPROPERTY()
	TArray<UCPP_AnswerButton*> AnswerButtonStorage;
};
