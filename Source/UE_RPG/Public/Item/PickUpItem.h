

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"
#include "PickUpItem.generated.h"

UCLASS()
class UE_RPG_API APickUpItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APickUpItem();

	

	FORCEINLINE	void SetWidgetVisibility(bool Visible) { ItemStateWidjet->SetVisibility(Visible); }
	FORCEINLINE class UWidgetComponent* GetWidgetComponent() { return ItemStateWidjet; }
	/*FORCEINLINE FItemInfo GetItemInfo() { return ItemInfo; }
	FORCEINLINE void SetItemInfo(FItemInfo iteminfo) { ItemInfo = iteminfo; }*/
protected:

	virtual void BeginPlay() override;

	/**item states*/
	EItemState ItemState = EItemState::EIS_UnEquipped;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info Struct")
		FItemInfo ItemInfo;*/

	/**component*/
	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class UStaticMeshComponent* PickUpMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Widget")
		class UWidgetComponent* ItemStateWidjet;

	UPROPERTY(EditAnywhere)
		UDataTable* ItemDataTable;

	
	/**item state*/
	UPROPERTY(EditAnywhere, Category = "Item state")
		FName ItemInfoID;/**same name*/

	UPROPERTY(EditAnywhere, Category = "Item state")
		int32 ItemAmount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Item state")
		class UItem* ItemRef;

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

public:	
	
	void InitializePickUpItem();

	void TakePicUp(class ACPP_Character* taker);
	

};
