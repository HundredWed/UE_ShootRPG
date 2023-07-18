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
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	EItemState ItemState = EItemState::EIS_UnEquipped;
private:
	


};
