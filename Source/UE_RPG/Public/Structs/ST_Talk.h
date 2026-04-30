#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs/ST_DialogueAnswer.h"
#include "ST_Talk.generated.h"

UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
	/**기본 상태 */
	None UMETA(DisplayName = "None (이벤트 없음)"),

	/** 퀘스트 완료 및 보상 지급 시스템 호출 */
	GiveQuestReward UMETA(DisplayName = "Quest Reward (퀘스트 보상 지급)"),

	/** 상점, 창고 등 특정 UI 위젯 팝업 */
	ShowSpecificUI UMETA(DisplayName = "Show UI (특정 UI 출력)"),

	/** 몬스터 스폰 또는 시네마틱 재생 등 */
	TriggerCinematic UMETA(DisplayName = "Play Cinematic (시네마틱 재생)"),

	/** 문이 열리는 등의 기믹 이벤트 */
	TriggerGimmick UMETA(DisplayName = "Gimmick (기믹 이벤트)")
};


USTRUCT(BlueprintType)
struct FTalkDialogue : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTalkDialogue() {};

	UPROPERTY(EditDefaultsOnly)
	FName NextRow;

	UPROPERTY(EditDefaultsOnly)
	FText Dialogue;

	UPROPERTY(EditDefaultsOnly)
	TArray<FAnswerDialogue> Answers;

	UPROPERTY(EditDefaultsOnly)
	EDialogueEventType DialogueEventType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "DialogueEventType == EDialogueEventType::TriggerGimmick", EditConditionHides))
	FName TriggerEventID = NAME_None;
};