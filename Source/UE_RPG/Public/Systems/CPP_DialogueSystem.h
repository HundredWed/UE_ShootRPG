#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Structs/ST_Talk.h"
#include "Structs/ST_NPCQuests.h"
#include "Structs/ST_NPC.h"
#include "UE_RPG/UtilityMecro.h"
#include "CPP_DialogueSystem.generated.h"

class UCPP_CustomInstance;

DECLARE_DELEGATE_TwoParams(FUpdateDialogueTextDelegate, const FText&, EDialogueEventType);
DECLARE_DELEGATE_TwoParams(FOnUpdateAnswerBoxDelegate, TArray<FAnswerDialogue>, bool);
DECLARE_DELEGATE(FOnQuitDialogueDelegate);
DECLARE_DELEGATE(FEndDialogueDelegate);

UCLASS(Blueprintable, BlueprintType)
class UE_RPG_API UCPP_DialogueSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCPP_DialogueSystem();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

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
	FEndDialogueDelegate EndDialogue;
	FOnUpdateAnswerBoxDelegate OnUpdateAnswerBox;
	FOnQuitDialogueDelegate OnQuitDialogue;


private:

	void RevertDialogue();

	void PrintDialogueNormal();
	void PrintDialogueJustTalk();
	void PrintDialogueQuest();
	void PrintDialogueLikeAbility();
	void PrintDialogueQuit();

	bool IsEndDialogue();

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
