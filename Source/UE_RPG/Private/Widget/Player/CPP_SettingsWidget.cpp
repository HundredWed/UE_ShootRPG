#include "Widget/Player/CPP_SettingsWidget.h"
#include "Components/Button.h"
#include "Systems/CPP_SaveDataSubsystem.h"
#include "Systems/CPP_UIEventHubSubsystem.h"


void UCPP_SettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (QuitGameButton)
    {
        QuitGameButton->OnClicked.AddDynamic(this, &UCPP_SettingsWidget::OnQuitGame);
    }

    UCPP_UIEventHubSubsystem* hub = GetGameInstance()->GetSubsystem<UCPP_UIEventHubSubsystem>();
    hub->OnSettingsWidgetToggleEvent.AddUObject(this, &UCPP_SettingsWidget::SetWidgetVisibility);

    SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_SettingsWidget::OnQuitGame()
{
    UGameInstance* GI = GetGameInstance();
    if (IsValid(GI))
    {
        UCPP_SaveDataSubsystem* SaveSubsystem = GI->GetSubsystem<UCPP_SaveDataSubsystem>();
        SaveSubsystem->SaveAndQuitGame();
    }
}
