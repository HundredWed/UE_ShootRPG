#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "MyBoxComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_RPG_API UMyBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UMyBoxComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMover(class UMover* NewMover);

	/*UPROPERTY(EditAnywhere)
	class AActor* ConnectMesh;*/

private:
	UPROPERTY(EditAnyWhere)
	FName ActorTag;

	class UMover* Mover;

	AActor* GetAcceptableActor() const;
};
