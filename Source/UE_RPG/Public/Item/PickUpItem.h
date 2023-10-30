

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "UE_RPG/UtilityMecro.h"
#include "PickUpItem.generated.h"

UCLASS()
class UE_RPG_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APickUpItem();
	
	FORCEINLINE bool IsValidWidget() { return bValidWidget; }
	FORCEINLINE class UWidgetComponent* GetWidgetComponent() { return ItemStateWidjet; }
	FORCEINLINE	void SetItemInfoID(FName itemid) { ItemInfoID = itemid; }
	FORCEINLINE	class UItem* GetPickUpItemRef() { return ItemRef; }


	void SetWidgetVisibility(bool Visible);

protected:

	virtual void BeginPlay() override;

	/**item states*/
	EItemState ItemState = EItemState::EIS_UnEquipped;


	/**component*/
	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class UStaticMeshComponent* PickUpMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SearchComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
		class UWidgetComponent* ItemStateWidjet;

	
	/**item state*/
	UPROPERTY(EditAnywhere, Category = "Item state")
		FName ItemInfoID;/**same name*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item state")
		int32 ItemAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item state")
		class UItem* ItemRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "Item state")
		UDataTable* ItemDataTable;

	UPROPERTY()
		class ACPP_Character* Player;


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

	bool bValidWidget = false;

public:	
	
	void InitializePickUpItem();

	UFUNCTION()
		void TakePickUp(class ACPP_Character* taker);
	

};
