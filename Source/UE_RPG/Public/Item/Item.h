#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Item.generated.h"



UCLASS()
class UE_RPG_API UItem : public UObject
{
	GENERATED_BODY()
	
public:	
	
	UItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		FItemInfoTable ItemInfoTable;

	bool bCopyItem;

	UFUNCTION()
		UItem* CreateItemCopy();

protected:

	UPROPERTY()
		class ACPP_Character* playerRef;

private:

};
