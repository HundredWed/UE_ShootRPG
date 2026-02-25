#include "Widget/Player/Quest/CPP_InProgressQuestsWidget.h"
#include "Systems/CPP_QuestSubsystem.h"
#include "Widget/NPC/Dialogue/CPP_QuestListButton.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UCPP_InProgressQuestsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* questSystem = GI->GetSubsystem<UCPP_QuestSubsystem>();
		questSystem->OnChangeQuestInfo.BindUObject(this, &UCPP_InProgressQuestsWidget::UpdateQuestInfo);
	}
	
	if (HiddenAnimation)
	{
		FWidgetAnimationDynamicEvent AnimDelegate;
		AnimDelegate.BindDynamic(this, &UCPP_InProgressQuestsWidget::OnHiddenAnimationFinished);
		BindToAnimationFinished(HiddenAnimation, AnimDelegate);
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_InProgressQuestsWidget::InitQuestList()
{
	UGameInstance* GI = GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_QuestSubsystem* questSystem = GI->GetSubsystem<UCPP_QuestSubsystem>();
		TArray<FQuest> quests = questSystem->GetProgressQuestsOfPlayer();

		for (FQuest quest : quests)
		{
			UCPP_QuestListButton* list = CreateWidget<UCPP_QuestListButton>(GetWorld(), QuestButtonListClass);
			list->InitQuestListButton(quest);
			list->OnButtonClicked.BindUObject(this, &UCPP_InProgressQuestsWidget::UpdateQuestText);
			list->AddToViewport();
			QuestListBox->AddChild(list);
			AllChildrenOfListBox.Add(quest.QuestID, list);
		}
	}
}

void UCPP_InProgressQuestsWidget::AddQuestList(const FQuest& quest)
{
	UCPP_QuestListButton* list = CreateWidget<UCPP_QuestListButton>(GetWorld(), QuestButtonListClass);
	list->InitQuestListButton(quest);
	list->AddToViewport();
	QuestListBox->AddChild(list);
	AllChildrenOfListBox.Add(quest.QuestID, list);
}

void UCPP_InProgressQuestsWidget::RemoveQuestList(const FName& questID)
{
	AllChildrenOfListBox.Remove(questID);
}

void UCPP_InProgressQuestsWidget::SetCustomVisibility(ESlateVisibility visibility)
{
	if (!VisibleAnimation || !HiddenAnimation)
	{
		return;
	}

	switch (visibility)
	{
	case ESlateVisibility::Visible:
		PlayAnimation(VisibleAnimation);
		SetVisibility(ESlateVisibility::Visible);
		break;
	case ESlateVisibility::Hidden:
		PlayAnimation(HiddenAnimation);
		break;
	default:
		break;
	}
}

void UCPP_InProgressQuestsWidget::UpdateQuestText(const FQuest& quest)
{
	QuestDescription->SetText(quest.QuestContent);

	UpdateQuestObjective(quest);
}

void UCPP_InProgressQuestsWidget::UpdateQuestObjective(const FQuest& quest)
{
	FText formatPattern;
	FText resultText;

	if (quest.QuestType >= EQuestType::EQT_ComBat)
	{
		if (quest.QuestState == EQuestState::EQS_ConditionClear)
		{
			formatPattern = FText::FromString(TEXT("{ObjectName}   {Current} / {Need} (완료)"));
		}
		else
		{
			formatPattern = FText::FromString(TEXT("{ObjectName}   {Current} / {Need}"));
		}

		FText currentCount = FText::AsNumber(quest.CurrentCount);
		FText needCount = FText::AsNumber(quest.NeedCount);

		resultText = FText::Format(formatPattern, quest.NeedObjectName, currentCount, needCount);		
	}
	else
	{

		if (quest.QuestState == EQuestState::EQS_ConditionClear)
		{
			formatPattern = FText::FromString(TEXT("{ObjectName} (완료)"));
		}
		else
		{
			formatPattern = FText::FromString(TEXT("{ObjectName}"));
		}

		resultText = FText::Format(formatPattern, quest.NeedObjectName);
	}

	QuestObjective->SetText(resultText);
}

void UCPP_InProgressQuestsWidget::UpdateQuestInfo(const FQuest& quest)
{
	UCPP_QuestListButton* button = *(AllChildrenOfListBox.Find(quest.QuestID));
	button->InitQuestListButton(quest);
}

void UCPP_InProgressQuestsWidget::OnHiddenAnimationFinished()
{
	SetVisibility(ESlateVisibility::Hidden);
}
