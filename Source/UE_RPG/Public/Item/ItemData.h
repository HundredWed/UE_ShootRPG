#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"


UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	EIS_Consumeable UMETA(DisplayName = "Consumeable"),
	EIS_Equipment UMETA(DisplayName = "Equipment"),
	EIS_QuestItems UMETA(DisplayName = "Quest Items"),
	EIS_Readables UMETA(DisplayName = "Readables"),
	EIS_Gabbable UMETA(DisplayName = "Grab Item")
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* IconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanStacked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText UseText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Interaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 ItemPrice;

};