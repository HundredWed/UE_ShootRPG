#include "UE_RPGGameModeBase.h"
#include "Systems/CPP_SaveDataSubsystem.h"

void AUE_RPGGameModeBase::StartPlay()
{
	Super::StartPlay();

    GetWorld()->GetTimerManager().SetTimer(
        AutoSaveTimerHandle,
        this,
        &AUE_RPGGameModeBase::TriggerAutoSave,
        AutoSaveDelay,
        true
    );
}

void AUE_RPGGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
    }

    Super::EndPlay(EndPlayReason);
}

void AUE_RPGGameModeBase::TriggerAutoSave()
{
    SCREENLOG(INDEX_NONE, 5.f, FColor::Blue, TEXT("저장 실행"));
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UCPP_SaveDataSubsystem* SaveSubsystem = GI->GetSubsystem<UCPP_SaveDataSubsystem>())
        {
            SaveSubsystem->SaveGameDataAsync();
        }
    }
}
