#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "Item.generated.h"


enum class EItemState : uint8
{
	EIS_UnEquipped,
	EIS_Equipped
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
