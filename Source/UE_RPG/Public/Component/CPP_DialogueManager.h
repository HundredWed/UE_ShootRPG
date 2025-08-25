// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/ST_Talk.h"
#include "Structs/ST_NPCQuests.h"
#include "Structs/ST_NPC.h"
#include "CPP_DialogueManager.generated.h"

class ACPP_Controller;
class UCPP_CustomInstance;

UENUM(BlueprintType)
enum class EDialogueState : uint8
{
	Normal,
	JustTalk,
	Quest,
	LikeAbility,
	Quit
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_DialogueManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_DialogueManager();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void InitDialogue(const FName& dialogueOwnerName);

	void PrintDialogue();
	void SelectedInteractType(EDialogueState state);
	void SelectedAnswer(const FName& rowName, bool questAccept);
	void SelectedQuest(const FQuest& quest);

	//DataTable
	FTalkDialogue GetTalkStruct();
	FTalkDialogue GetQuestDialogueStruct();
	FNPCDialogue GetNPCStruct();

protected:
	
	virtual void BeginPlay() override;

private:

	void RevertDialogue();
	void RevertToMain();
	void SetQuestList();
	void SetAnswerBox(const FTalkDialogue& dialouge);

	void PrintDialogueNormal();
	void PrintDialogueJustTalk();
	void PrintDialogueQuest();
	void PrintDialogueLikeAbility();
	void PrintDialogueQuit();

private:

	FName DialogueOwnerName;

	//DataTable
	FName DatatableRowName;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* TalkDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* QuestDialogueDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	UDataTable* NPCDataTable;


	EDialogueState DialogueState = EDialogueState::Normal;

	bool bActivateAnswerBox;		
	bool bSelectedQuest;		
	bool bTalkEnd;		

	UPROPERTY()
	UCPP_CustomInstance* CustomInstance;

	ACPP_Controller* PlayerController;
};
