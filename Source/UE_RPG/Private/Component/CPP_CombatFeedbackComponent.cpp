#include "Component/CPP_CombatFeedbackComponent.h"
#include "Widget/NPC/CPP_DamageActor.h"


UCPP_CombatFeedbackComponent::UCPP_CombatFeedbackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCPP_CombatFeedbackComponent::BeginPlay()
{
	Super::BeginPlay();

	StoreDamageUI();
}

void UCPP_CombatFeedbackComponent::StoreDamageUI()
{
	UWorld* world = GetWorld();
	const int32 amount = 30;

	DamageUIActors.Reserve(amount);

	if (IsValid(world) && IsValid(DamageUIActorClass))
	{
		for (int32 i = 0; i < amount; i++)
		{
			ACPP_DamageActor* damageActor = world->SpawnActor<ACPP_DamageActor>(DamageUIActorClass);
			DamageUIActors.Push(damageActor);
		}
	}
}

void UCPP_CombatFeedbackComponent::SpawnDamageUI(const FDamageReceipt& receipt)
{
	if (ACPP_DamageActor* damageActor = GetDamageActor())
	{
		damageActor->UpdateDamageActor(receipt);
	}	
}

ACPP_DamageActor* UCPP_CombatFeedbackComponent::GetDamageActor()
{
	CurrentIndex = CurrentIndex % DamageUIActors.Num();
	ACPP_DamageActor* nextUI = DamageUIActors[CurrentIndex];
	
	if (nextUI && nextUI->IsActivateUI())
	{
		UE_LOG(LogTemp, Display, TEXT("데미지 ui 전부 소진!! 확장 실행"));
		if (UWorld* world = GetWorld())
		{
			const int32 addSize = 5;
			const int32 oldSize = DamageUIActors.Num();

			for (int32 i = 0; i < addSize; i++)
			{
				ACPP_DamageActor* damageActor = world->SpawnActor<ACPP_DamageActor>(DamageUIActorClass);
				DamageUIActors.Push(damageActor);
			}

			nextUI = DamageUIActors[oldSize];
			CurrentIndex = oldSize + 1;
		}
		
	}
	else
	{
		CurrentIndex++;
	}
	

	return nextUI;
}