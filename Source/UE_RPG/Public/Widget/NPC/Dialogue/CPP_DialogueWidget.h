// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ST_DialogueAnswer.h"
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

UCLASS()
class UE_RPG_API UCPP_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void InitDialogueWidget();
	
	void UpdateDialogueText(const FText& text);
	void UpdateAnswerBox(TArray<FAnswerDialogue> answers);

	UFUNCTION(BlueprintImplementableEvent)
	bool ActivateDialogueSubBox(ESlateVisibility Invisibility);

	UFUNCTION(BlueprintImplementableEvent)
	bool ActivateDialogueMainBox(ESlateVisibility Invisibility);

private:

	void SetInteractButton(const FNPCDialogue& npcInfo);
	void SetMainBox(const FNPCDialogue& npcInfo);
	void SetSubBox(const FNPCDialogue& npcInfo);
	void InteractButtonEvent(EInteractType interactType);
	void InitQuestSystem();
	void ActivateAnswerBox(bool bActivate);
	void AnswerEvent();
	void SetVisibilityQuestListBox(ESlateVisibility visibility);

	bool HasAvailableQuest(const FNPCDialogue& npcInfo);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* InteractContentsBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UHorizontalBox* DialogueSubBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_QuestListBox* QuestListBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_AnswerBox* AnswerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* NPCName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* DialogueText;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EInteractType, TSubclassOf<UCPP_DialogueButtonBase>> InteractButtons;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<UCPP_AnswerButton> AnswerButtonClass;


	TObjectPtr<UCPP_DialogueSystem> DialogueSystem;

	FName NPCID;

};
