#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Interface/CPP_InteractInterface.h"
#include "Engine/DataTable.h"
#include "UE_RPG/UtilityMecro.h"
#include "PickUpItem.generated.h"

class ACPP_Character;

UCLASS()
class UE_RPG_API APickUpItem : public AActor, public ICPP_InteractInterface
{
	GENERATED_BODY()
	
public:	
	APickUpItem();
	virtual void OnConstruction(const FTransform& Transform) override;

	void SetWidgetVisibility(bool Visible);
	void InitializePickUpItem();

	virtual void RequestInteract(AActor* interactor) override;
	virtual void OnBeginLookAt() override;
	virtual void OnEndLookAt() override;

protected:

	virtual void BeginPlay() override;

	/**component*/
	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class UStaticMeshComponent* PickUpMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SearchComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* ItemStateWidget;

	
	/**item state*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemInfo")
	FDataTableRowHandle ItemInfoHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemInfo")
	int32 ItemAmount = 1;

	UPROPERTY()
	EItemCategory ItemCategory;

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

private:

	/**OnConstruction 최적화 용*/
	FName PevItemID = NAME_None;
};
