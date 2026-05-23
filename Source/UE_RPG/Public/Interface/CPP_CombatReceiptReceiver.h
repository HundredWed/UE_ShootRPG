#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_CombatReceiptReceiver.generated.h"

struct FDamageReceipt;

UINTERFACE(MinimalAPI)
class UCPP_CombatReceiptReceiver : public UInterface
{
	GENERATED_BODY()
};


class UE_RPG_API ICPP_CombatReceiptReceiver
{
	GENERATED_BODY()


public:

	virtual void SubmitReceipt(const FDamageReceipt& receipt) {}
};
