#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ST_Talk.h"
#include "Structs/ST_NPCQuests.h"
#include "Structs/ST_NPC.h"
#include "CPP_DialogueSystem.generated.h"

class UCPP_CustomInstance;

DECLARE_DELEGATE_OneParam(FUpdateDialogueTextDelegate, const FText&);
DECLARE_DELEGATE_OneParam(FCreateAnswerBoxDelegate, TArray<FAnswerDialogue>);
DECLARE_DELEGATE(FOnQuitDialogueDelegate);

UCLASS(Blueprintable, BlueprintType)
class UE_RPG_API UCPP_DialogueSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCPP_DialogueSystem();

	void InitDialogue(const FName& dialogueOwnerName);

	void PrintDialogue();
	void SelectedInteractType(EInteractType state);
	void SelectedAnswer(const FName& rowName);
	void SelectedQuest(const FQuest& quest);

	//DataTable
	FTalkDialogue GetTalkStruct();
	FTalkDialogue GetQuestDialogueStruct();
	FNPCDialogue GetNPCStruct();

	FUpdateDialogueTextDelegate UpdateDialogueText;
	FCreateAnswerBoxDelegate CreateAnswerBox;
	FOnQuitDialogueDelegate OnQuitDialogue;


private:

	void RevertDialogue();

	void PrintDialogueNormal();
	void PrintDialogueJustTalk();
	void PrintDialogueQuest();
	void PrintDialogueLikeAbility();
	void PrintDialogueQuit();

private:

	FName DialogueOwnerName;

	//DataTable
	FName DatatableRowName;

	UPROPERTY()
	UDataTable* TalkDataTable;

	UPROPERTY()
	UDataTable* QuestDialogueDataTable;

	UPROPERTY()
	UDataTable* NPCDataTable;


	EInteractType DialogueState = EInteractType::Normal;

	bool bActivateAnswerBox;
	bool bSelectedQuest;
	bool bTalkEnd;
};
