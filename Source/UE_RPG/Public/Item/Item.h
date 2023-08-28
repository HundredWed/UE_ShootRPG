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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItemInfo ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
		FName ItemInfoID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		FText Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		FText Description;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		bool bCanBeUsed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		bool bCanStacked;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		FText UseText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		FText Interaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		int32 ItemPrice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		EItemCategory ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		int32 ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset Data")
		UStaticMesh* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset Data")
		UTexture2D* IconTexture;

	UFUNCTION()
		UItem* CreateItemCopy();
	
protected:

	

private:

	



};
