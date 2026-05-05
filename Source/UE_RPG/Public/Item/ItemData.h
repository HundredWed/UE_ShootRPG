#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class UCPP_ConsumptionItemDataAsset;

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	EIC_Consumable UMETA(DisplayName = "Consumable"),
	EIC_Equipment UMETA(DisplayName = "Equipment"),
	EIC_QuestItems UMETA(DisplayName = "Quest Items"),
	EIC_ReadItem UMETA(DisplayName = "Read Item"),
	EIC_Combinables UMETA(DisplayName = "Combinable"),
	EIC_Gabbable UMETA(DisplayName = "Grab Item"),
	EIC_Gold UMETA(DisplayName = "Gold"),

	EIC_None UMETA(DisplayName = "NoneItem")
};

USTRUCT(BlueprintType)
struct FItemInfoTable : public FTableRowBase
{
	GENERATED_BODY()
		
public:
	FItemInfoTable() {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ItemInfoID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (MultiLine = "true"))
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	bool bCanBeUsed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	bool bCanStacked;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText UseText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText Interaction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 ItemPrice;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float Weight;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data")
	EItemCategory ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIC_Combinables", EditConditionHides))
	FName CombineResultID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIC_Consumable", EditConditionHides))
	TObjectPtr<UCPP_ConsumptionItemDataAsset> ItemLogicAsset;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	TSoftObjectPtr<UStaticMesh> ItemMesh; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
	UTexture2D* IconTexture;
};