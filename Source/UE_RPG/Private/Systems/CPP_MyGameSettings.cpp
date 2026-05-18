#include "Systems/CPP_MyGameSettings.h"

UDataTable* UCPP_MyGameSettings::LoadDataTableSafely(const TSoftObjectPtr<UDataTable>& SoftTablePtr)
{
    if (SoftTablePtr.IsValid() || SoftTablePtr.IsPending())
    {
        return SoftTablePtr.LoadSynchronous();
    }

    UE_LOG(LogTemp, Warning, TEXT("MyGameSettings: 데이터 테이블 경로가 비어있거나 로드할 수 없습니다."));
    return nullptr;
}

UClass* UCPP_MyGameSettings::LoadWorldObjSubClassSafely(const TSoftClassPtr<AActor>& SoftTablePtr)
{
    if (SoftTablePtr.IsValid() || SoftTablePtr.IsPending())
    {
        return SoftTablePtr.LoadSynchronous();
    }

    UE_LOG(LogTemp, Warning, TEXT("MyGameSettings: 데이터 테이블 경로가 비어있거나 로드할 수 없습니다."));
    return nullptr;
}
