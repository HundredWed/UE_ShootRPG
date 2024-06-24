#pragma once

#include "CoreMinimal.h"
#include "NPC/EnemyBase.h"
#include "CPP_Enemy_Gunner.generated.h"


UCLASS()
class UE_RPG_API ACPP_Enemy_Gunner : public AEnemyBase
{
	GENERATED_BODY()
public:

	ACPP_Enemy_Gunner();

	virtual void BeginPlay() override;
	virtual void Combat() override;

	void GunTrace();
};
