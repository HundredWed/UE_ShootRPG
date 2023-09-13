#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"


enum class EItemState : uint8
{
	EIS_UnEquipped,
	EIS_Equipped
};

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

	UPROPERTY(EditAnywhere, Category = "Item Data")
		FName ItemInfoID;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FText Name;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FText Description;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		bool bCanBeUsed;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		bool bCanStacked;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FText UseText;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		FText Interaction;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		int32 ItemPrice;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		float Weight;
	UPROPERTY(EditAnywhere, Category = "Item Data")
		int32 ATK;

	UPROPERTY(EditAnywhere, Category = "ItemType Data")
		TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, Category = "ItemType Data")
		EItemCategory ItemType;
	UPROPERTY(EditAnywhere, Category = "ItemType Data")
		int32 ConsumeValue;


	UPROPERTY(EditAnywhere, Category = "Asset Data")
		UStaticMesh* ItemMesh;
	UPROPERTY(EditAnywhere, Category = "Asset Data")
		UTexture2D* IconTexture;
};