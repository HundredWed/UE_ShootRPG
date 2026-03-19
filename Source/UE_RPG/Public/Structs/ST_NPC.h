#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ST_NPC.generated.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	Normal,
	JustTalk,
	Quest,
	LikeAbility,
	Revert,
	Quit
};

USTRUCT(BlueprintType)
struct FNPCDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:
	FNPCDialogue() {};

	/*UPROPERTY(EditDefaultsOnly)
	FName NPCID;*/

	UPROPERTY(EditDefaultsOnly)
	FText NPCName;

	UPROPERTY(EditDefaultsOnly)
	FText BasicDialogue;

	UPROPERTY(EditDefaultsOnly)
	TArray<EInteractType> CanInteractTypes;
};