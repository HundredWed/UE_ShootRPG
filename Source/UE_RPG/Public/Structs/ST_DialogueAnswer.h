#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_DialogueAnswer.generated.h"

USTRUCT(BlueprintType)
struct FAnswerDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:
	FAnswerDialogue() {};

	UPROPERTY(EditDefaultsOnly)
	FText Answer;

	UPROPERTY(EditDefaultsOnly)
	FName RowName;

	UPROPERTY(EditDefaultsOnly)
	bool bAccept;

};