#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"


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

UCLASS()
class UE_RPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItem();
	

	FORCEINLINE	void SetWidgetVisibility(bool Visible);
	FORCEINLINE class UWidgetComponent* GetWidgetComponent() { return ItemStateWidjet; }
	FORCEINLINE FItemInfo GetItemInfo() { return ItemInfo; }
	FORCEINLINE void SetItemInfo(FItemInfo iteminfo) { ItemInfo = iteminfo; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/**item states*/
	EItemState ItemState = EItemState::EIS_UnEquipped;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info Struct")
		FItemInfo ItemInfo;

	/**component*/
	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
		class UWidgetComponent* ItemStateWidjet;

	
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex);

	virtual void SetItemState(EItemState Stat);

public:

	

private:

	



};
