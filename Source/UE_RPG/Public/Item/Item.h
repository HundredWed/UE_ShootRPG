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
		float Weight;

	UPROPERTY(VisibleAnywhere, Category = "ItemType Data")
		TSubclassOf<AActor> ItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemType Data")
		EItemCategory ItemType;

	UPROPERTY(EditAnywhere, Category = "ItemType Data")
		FName CombinResultID;

	UPROPERTY(VisibleAnywhere, Category = "ItemType Data")
		int32 ConsumeValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
		int32 ATK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset Data")
		UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Asset Data")
		USkeletalMesh* ItemSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset Data")
		UTexture2D* IconTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset Data")
		class UParticleSystem* FireParticle;

	bool bCopyItem;

	UFUNCTION()
		UItem* CreateItemCopy();

protected:

	UPROPERTY()
		class ACPP_Character* playerRef;

private:

};
