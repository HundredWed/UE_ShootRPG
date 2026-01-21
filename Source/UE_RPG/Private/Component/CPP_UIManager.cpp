#include "Component/CPP_UIManager.h"
#include "Blueprint/UserWidget.h"

#include "Widget/MainPanelWidget.h"
#include "Systems/CPP_DialogueSystem.h"
#include "Widget/NPC/Dialogue/CPP_DialogueWidget.h"
#include "CPP_Character.h"
#include "CPP_Controller.h"

UCPP_UIManager::UCPP_UIManager()
{
	//PrimaryComponentTick.bCanEverTick = true;
}

void UCPP_UIManager::SetMainWidget(EWidgetType type)
{
	HideCurrentWidget();

	switch (type)
	{
	case EWidgetType::Player:
		SetMainWidgetToPlayer();
		break;
	case EWidgetType::NPCDialogue:
		SetMainWidgetToDialogue();
		break;
	default:
		break;
	}
}

void UCPP_UIManager::HideCurrentWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCPP_UIManager::SwitchToPlayerWidget()
{
	SetMainWidget(EWidgetType::Player);
}

void UCPP_UIManager::SetMainWidgetToPlayer()
{
	if (TObjectPtr<UUserWidget>* wd = Widgets.Find(EWidgetType::Player))
	{
		UMainPanelWidget* playerWG = Cast<UMainPanelWidget>((*wd));
		playerWG->SetVisibility(ESlateVisibility::Visible);
		//playerWG->UpdatePlayerInfo();

		ACPP_Character* player = Cast<ACPP_Character>(PlayerController->GetLocalPlayer());
		//해당 구간 리펙토링 필요
		playerWG->InitState(player->GetPlayerLevel(), player->GetPlayerHealth(), player->GetPlayerMaxHealth(), player->GetPlayerMaxMana(), player->GetPlayerStamina());

		CurrentWidgetType = EWidgetType::Player;
		CurrentWidget = (*wd);
	}	
}

void UCPP_UIManager::SetMainWidgetToDialogue()
{
	if (TObjectPtr<UUserWidget>* wd = Widgets.Find(EWidgetType::NPCDialogue))
	{		
		UCPP_DialogueWidget* npcDialogueWidget = Cast<UCPP_DialogueWidget>((*wd));
		npcDialogueWidget->InitDialogueWidget();
		CurrentWidgetType = EWidgetType::NPCDialogue;
		CurrentWidget = (*wd);
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

	SetMainWidgetToPlayer();


	UGameInstance* GI = PlayerController->GetGameInstance();
	if (IsValid(GI))
	{
		UCPP_DialogueSystem* dialogue = GI->GetSubsystem<UCPP_DialogueSystem>();
		dialogue->OnQuitDialogue.BindUObject(this, &UCPP_UIManager::SwitchToPlayerWidget);
	}	
}

