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
	EIS_Combinables UMETA(DisplayName = "Combinable"),
	EIS_Gabbable UMETA(DisplayName = "Grab Item"),
	EIS_Gold UMETA(DisplayName = "Gold"),

	EIS_None UMETA(DisplayName = "NoneItem")
};


/**If there is a change in this structure, 
the Item class's header file, CreateItemCopy() function, 
and the PickUpItem class's InitializePickUpItem() function should also be updated accordingly.*/
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


	UPROPERTY(EditAnywhere, Category = "ItemType Data")
		TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, Category = "ItemType Data")
		EItemCategory ItemType;

	UPROPERTY(EditAnywhere, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Combinables", EditConditionHides))
		FName CombinResultID;
	UPROPERTY(EditAnywhere, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Consumeable", EditConditionHides))
		int32 ConsumeValue;
	UPROPERTY(EditAnywhere, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Equipment", EditConditionHides))
		int32 ATK;
	


	UPROPERTY(EditAnywhere, Category = "Asset Data")
		UStaticMesh* ItemMesh;
	UPROPERTY(EditAnywhere, Category = "Asset Data")
		UTexture2D* IconTexture;
};