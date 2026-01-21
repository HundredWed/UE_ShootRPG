#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_DialogueAnswer.generated.h"

UENUM(BlueprintType)
enum class EAnswerType : uint8
{
	Normal,
	Positive,
	Negative
};

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
	EAnswerType AnswerType;

};