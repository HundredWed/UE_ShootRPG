#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyBase.h"
#include "CPP_Enemy_Cannon.generated.h"

UCLASS()
class UE_RPG_API ACPP_Enemy_Cannon : public AEnemyBase
{
	GENERATED_BODY()
	
public:

	ACPP_Enemy_Cannon();

	virtual void BeginPlay() override;
	virtual void Combat() override;

	void AttackFunc();
};
