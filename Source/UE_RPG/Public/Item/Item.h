#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	/**Currently deemed unnecessary.*/
	/**However, left for future reference.*/
	/*FORCEINLINE void SetIsGrabbable(bool grabbable) { isGrabbalble = grabbable; }
	FORCEINLINE bool GetIsGrabbable() { return isGrabbalble; }
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraValue", meta = (AllowPrivateAccess = "true"))
		bool isGrabbalble = true;*/

	

	FORCEINLINE	void SetWidgetVisibility(bool Visible);
	FORCEINLINE class UWidgetComponent* GetWidgetComponent() { return ItemStateWidjet; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	EItemState ItemState = EItemState::EIS_UnEquipped;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Item Component")
		class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Item Widget")
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

	/**common item states*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common item states", meta = (AllowPrivateAccess = "true"))
		FString ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Common item states", meta = (AllowPrivateAccess = "true"))
		int32 Price;



};
