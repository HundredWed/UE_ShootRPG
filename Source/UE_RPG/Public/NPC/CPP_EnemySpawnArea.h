#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_EnemySpawnArea.generated.h"

class ACPP_Character;
class AEnemyBase;
class USphereComponent;

UCLASS()
class UE_RPG_API ACPP_EnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
public:
	ACPP_EnemySpawnArea();

	//virtual void Tick(float DeltaTime) override;

	void SpawnEnemy();
	void FocusTarget();
	void SetStateNormal();
	void Encounter();
	void TargetIsNotValid();
	void EnemyDeathCount();
	void CreateEnemy();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	/*UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);*/

public:

	//Component
	UPROPERTY(VisibleAnywhere, Category = "Area Component")
		USphereComponent* SpawnArea;


	//Area Info
	UPROPERTY(EditAnywhere, Category = "Area Info")
		float ValidDis = 300.f;

	
private:

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		ACPP_Character* Target;

	UPROPERTY(EditAnywhere, Category = "Area Info")
		TArray<AEnemyBase*> Enemys;

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		TArray<TSubclassOf<AEnemyBase>> EnemyClass;

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		TArray<AActor*> SpawnPoint;

	FVector CenterPos;
	int32 EnemysNum;
};
