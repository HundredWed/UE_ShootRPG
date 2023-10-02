

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
	
	FORCEINLINE class UWidgetComponent* GetWidgetComponent() { return ItemStateWidjet; }
	FORCEINLINE	void SetItemInfoID(FName itemid) { ItemInfoID = itemid; }
	FORCEINLINE	class UItem* GetPickUpItemRef() { return ItemRef; }

	// 이거는 FORCEINCLINE 쓰면 안됨 하위 함수까지 전부다 FORCEINLINE 적용됨 FORCEINCLINE은 주의해서 사용할것
	// 그리고 이런종류는 내부에서 Validate를 해줘야함 [10/03/2023 Sunny8747]
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

public:	
	
	void InitializePickUpItem();

	UFUNCTION()
		void TakePickUp(class ACPP_Character* taker);
	

};
