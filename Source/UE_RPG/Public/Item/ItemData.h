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

UENUM(BlueprintType)
enum class EWeaponAbilityID : uint8
{
	EWA_Normal UMETA(DisplayName = "NormalRifle"),
	EWA_Red UMETA(DisplayName = "RedRifle"),

	EIS_AbilityNone UMETA(DisplayName = "NoneItem")
};


/**If there is a change in this structure, 
the Item class's header file, CreateItemCopy() function, 
and the PickUpItem class's InitializePickUpItem() function should also be updated accordingly.*/
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
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
		TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data")
		EItemCategory ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Combinables", EditConditionHides))
		FName CombinResultID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Consumeable", EditConditionHides))
		int32 ConsumeValue;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Equipment", EditConditionHides))
		int32 ATK;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemType Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Equipment", EditConditionHides))
		EWeaponAbilityID WeaponAbilityID;
	


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
		UStaticMesh* ItemMesh; 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
		USkeletalMesh* ItemSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data")
		UTexture2D* IconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Data", meta = (EditCondition = "ItemType == EItemCategory::EIS_Equipment", EditConditionHides))
		class UParticleSystem* FireParticle;

public:

	FItemInfoTable& operator=(const FItemInfoTable* other)
	{
		ItemInfoID = other->ItemInfoID;
		Name = other->Name;
		Description = other->Description;
		bCanBeUsed = other->bCanBeUsed;
		bCanStacked = other->bCanStacked;
		UseText = other->UseText;
		Interaction = other->Interaction;
		ItemPrice = other->ItemPrice;
		Weight = other->Weight;
		ATK = other->ATK;

		/**itemtype data*/
		ItemType = other->ItemType;
		ConsumeValue = other->ConsumeValue;
		CombinResultID = other->CombinResultID;
		if (other->ItemClass)
		{
			ItemClass = other->ItemClass;
		}
		WeaponAbilityID = other->WeaponAbilityID;

		/**asset data*/
		ItemMesh = other->ItemMesh;
		IconTexture = other->IconTexture;
		ItemSkeletalMesh = other->ItemSkeletalMesh;
		FireParticle = other->FireParticle;

		return *this;
	}

};