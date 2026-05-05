#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CPP_UIEventHubSubsystem.generated.h"


DECLARE_DELEGATE(FOnRequestShowCursorEvent);
DECLARE_DELEGATE(FOnRequestHideCursorEvent);

DECLARE_MULTICAST_DELEGATE(FOnInventoryToggleEvent)
DECLARE_MULTICAST_DELEGATE(FOnQuestListToggleEvent)
DECLARE_MULTICAST_DELEGATE(FOnSettingsWidgetToggleEvent)

UCLASS()
class UE_RPG_API UCPP_UIEventHubSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	FOnRequestShowCursorEvent OnRequestShowCursor;
	FOnRequestHideCursorEvent OnRequestHideCursor;
	

	FOnInventoryToggleEvent OnInventoryToggleEvent;
	FOnQuestListToggleEvent OnQuestListToggleEvent;
	FOnSettingsWidgetToggleEvent OnSettingsWidgetToggleEvent;

};
