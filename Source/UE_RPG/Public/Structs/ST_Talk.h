#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/ST_DialogueAnswer.h"
#include "ST_Talk.generated.h"

USTRUCT(BlueprintType)
struct FTalkDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTalkDialogue() {};

	UPROPERTY(EditDefaultsOnly)
	FName RowName;

	UPROPERTY(EditDefaultsOnly)
	FName QuestClearRow;

	UPROPERTY(EditDefaultsOnly)
	FName QuestProgressRow;

	UPROPERTY(EditDefaultsOnly)
	FText Dialogue;

	UPROPERTY(EditDefaultsOnly)
	bool EndDialogue;

	UPROPERTY(EditDefaultsOnly)
	TArray<FAnswerDialogue> Answers;

};