// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Consumable/CPP_StatRestoreLogicAsset.h"
#include "Interface/CPP_StatInterface.h"

bool UCPP_StatRestoreLogicAsset::ExecuteLogic(AActor* Target)
{
    if (ICPP_StatInterface* target = Cast<ICPP_StatInterface>(Target))
    {
        target->OnRestore(RestoreType, RestoreAmount);
        DISPLAYLOG(TEXT("회복 아이템 사용!"));
        
        return true;
    }
    
    return false;
}
