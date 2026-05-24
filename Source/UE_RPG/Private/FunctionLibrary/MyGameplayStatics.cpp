#include "FunctionLibrary/MyGameplayStatics.h"
#include "NPC/HitEventInterface.h"
#include "Component/CPP_StatComponent.h"

void UMyGameplayStatics::ApplyDamage(AActor* target, const FDamageReceipt& receipt, AActor* damageCauser)
{
	if (!IsValid(target))
		return;

	if (IHitEventInterface* hitEvent = Cast<IHitEventInterface>(target))
	{
		hitEvent->ExecuteHitEvent(receipt, damageCauser);
	}
}

void UMyGameplayStatics::ApplyRangeDamage(AActor* target, const FDamageReceipt& receipt, const float range, AActor* damageCauser)
{
	if (!IsValid(target))
		return;
	
	UWorld* world = target->GetWorld();
	if (!IsValid(world))
		return;

	FVector originLocation = target->GetActorLocation();

	TArray<FOverlapResult> overlapResults;
	FCollisionShape sphere = FCollisionShape::MakeSphere(range);
	FCollisionQueryParams queryParams;

	if (damageCauser)
	{
		queryParams.AddIgnoredActor(damageCauser);
	}

	bool bHasOverlaps = world->OverlapMultiByChannel(
		overlapResults,
		originLocation,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere,
		queryParams
	);

	if (bHasOverlaps)
	{
		TSet<AActor*> damagedActors;

		for (const FOverlapResult& Result : overlapResults)
		{
			AActor* overlappedActor = Result.GetActor();

			if (overlappedActor && !damagedActors.Contains(overlappedActor))
			{
				FHitResult TraceHit;
				bool bHitObstacle = world->LineTraceSingleByChannel(
					TraceHit,
					originLocation,
					overlappedActor->GetActorLocation(),
					ECC_GameTraceChannel2,
					queryParams
				);

				
				if (!bHitObstacle || TraceHit.GetActor() == overlappedActor)
				{
					if (IHitEventInterface* HitEvent = Cast<IHitEventInterface>(overlappedActor))
					{
						HitEvent->ExecuteHitEvent(receipt, damageCauser);
					}

					damagedActors.Add(overlappedActor);
				}
			}
		}
	}
}

void UMyGameplayStatics::ApplyDotDamage(AActor* target, const FDamageReceipt& receipt, const float delay, const float duration, AActor* damageCauser)
{
	if (!IsValid(target))
		return;

	if (UCPP_StatComponent* StatComp = target->FindComponentByClass<UCPP_StatComponent>())
	{
		StatComp->ExecuteDotDamage(receipt.Damage, delay, duration);
	}
}
