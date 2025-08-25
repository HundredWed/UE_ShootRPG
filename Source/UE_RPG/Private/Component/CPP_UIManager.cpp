#include "Component/CPP_UIManager.h"
#include "Blueprint/UserWidget.h"

#include "Widget/MainPanelWidget.h"
#include "Component/CPP_DialogueManager.h"
#include "CPP_Character.h"
#include "CPP_Controller.h"

UCPP_UIManager::UCPP_UIManager()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void UCPP_UIManager::SetMainWidget(EWidgetType widgetType)
{
	if (CurrentWidgetType != widgetType)
	{
		Widgets[CurrentWidgetType]->SetVisibility(ESlateVisibility::Hidden);
	}
	
	switch (widgetType)
	{
	case EWidgetType::Player:
		SetMainWidgetToPlayer();
		break;
	case EWidgetType::NPCDialogue:
		SetMainWidgetToDialogue(PlayerController->GetDialogueManager()->GetNPCStruct());
		break;
	default:
		break;
	}
}

void UCPP_UIManager::SetMainWidgetToPlayer()
{
	if (TObjectPtr<UUserWidget>* wd = Widgets.Find(EWidgetType::Player))
	{
		UMainPanelWidget* playerWG = Cast<UMainPanelWidget>((*wd));
		playerWG->SetVisibility(ESlateVisibility::Visible);
		//playerWG->UpdatePlayerInfo();

		CurrentWidgetType = EWidgetType::Player;
	}	
}

void UCPP_UIManager::SetMainWidgetToDialogue(const FNPCDialogue& npcDialogue)
{
	if (TObjectPtr<UUserWidget>* wd = Widgets.Find(EWidgetType::NPCDialogue))
	{		
		/*UDialogueWidget* npcDialogueWidget = Cast<UDialogueWidget>((*wd));
		*npcDialogueWidget로 초기화
		mainWidget->SetVisibility(ESlateVisibility::Visible);*/

		CurrentWidgetType = EWidgetType::NPCDialogue;
	}
}

void UCPP_UIManager::SetDialogueWidget()
{
}

void UCPP_UIManager::SetAnswerBox(TArray<FAnswerDialogue> answers)
{
	if (CurrentWidgetType == EWidgetType::NPCDialogue)
	{
		if (TObjectPtr<UUserWidget>* wd = Widgets.Find(EWidgetType::NPCDialogue))
		{
			//UDialogueWidget* npcDialogueWidget = Cast<UDialogueWidget>((*wd));
			//npcDialogueWidget->UpdateAnswerBox(answers);
		}
	}
}

void UCPP_UIManager::UpdateDialogueText(const FText& text)
{
	if (CurrentWidgetType == EWidgetType::NPCDialogue)
	{
		if (TObjectPtr<UUserWidget>* wd = Widgets.Find(EWidgetType::NPCDialogue))
		{
			//UDialogueWidget* npcDialogueWidget = Cast<UDialogueWidget>((*wd));
			//npcDialogueWidget->UpdateSubText(text);
		}
	}
}

void UCPP_UIManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ACPP_Controller>(GetOwner());

	for (auto widgetClass : WidgetClasses)
	{
		if (widgetClass.Value)
		{
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), widgetClass.Value);
			widget->AddToViewport();
			widget->SetVisibility(ESlateVisibility::Hidden);
			Widgets.Add(widgetClass.Key, widget);
		}
	}

	//메인 초기화
	if (TObjectPtr<UUserWidget>* main = Widgets.Find(EWidgetType::Player))
	{
		UMainPanelWidget* mainWidget = Cast<UMainPanelWidget>((*main));
		mainWidget->SetVisibility(ESlateVisibility::Visible);

		ACPP_Character* player = Cast<ACPP_Character>(PlayerController->GetLocalPlayer());
		mainWidget->InitState(player->GetPlayerLevel(), player->GetPlayerHealth(), player->GetPlayerMaxHealth(), player->GetPlayerMaxMana(), player->GetPlayerStamina());
	}
}

