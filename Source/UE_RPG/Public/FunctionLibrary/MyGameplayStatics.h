#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Structs/ST_DamageFeedback.h"
#include "MyGameplayStatics.generated.h"


UCLASS()
class UE_RPG_API UMyGameplayStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	static void ApplyDamage(AActor* target, const FDamageReceipt& receipt, AActor* damageCauser);
    
	UFUNCTION(BlueprintCallable)
	static void ApplyRangeDamage(AActor* target, const FDamageReceipt& receipt, const float range, AActor* damageCauser);

	UFUNCTION(BlueprintCallable)
	static void ApplyDotDamage(AActor* target, const FDamageReceipt& receipt, const float delay, const float duration, AActor* damageCauser);
};
