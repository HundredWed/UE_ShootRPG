#include "Widget/NPC/Dialogue/CPP_QuestListButton.h"

void UCPP_QuestListButton::InitQuestListButton(const FQuest& questInfo, bool bShowProgress)
{
	QuestInfo = questInfo;

	FText questName = QuestInfo.QuestName;
	
	if (bShowProgress)
	{
		questName = SetFormatText();
	}
	
	UpdateButtonText(questName);
}

void UCPP_QuestListButton::DialogueButtonEvent()
{
	Super::DialogueButtonEvent();

	if (OnButtonClicked.IsBound())
	{
		OnButtonClicked.Execute(QuestInfo);
	}
}

FText UCPP_QuestListButton::SetFormatText()
{
	FText questName = QuestInfo.QuestName;
	FText FormatPattern = FText::FromString(TEXT("{0}({1})"));

	switch (QuestInfo.QuestState)
	{
	case EQuestState::EQS_InProgress:
		return FText::Format(FormatPattern, questName, FText::FromString(TEXT("진행중")));
		break;
	case EQuestState::EQS_ConditionClear:
		return FText::Format(FormatPattern, questName, FText::FromString(TEXT("완료가능")));
		break;
	}

	return FText::FromString(TEXT("유효하지 않습니다."));
}
