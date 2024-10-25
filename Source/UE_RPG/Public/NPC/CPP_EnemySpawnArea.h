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
	void Encounter();
	void TargetIsNotValid();
	void EnemyDeathCount(const int32 arrNum);
	void EnemySpawnCount(const int32 arrNum);
	void CreateEnemy();

	UFUNCTION()
		ACPP_Character* GetTarget() { return Target; }

protected:

	virtual void BeginPlay() override;

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

public:

	//Component
	UPROPERTY(VisibleAnywhere, Category = "Area Component")
		USphereComponent* SpawnArea;


	//Area Info
	UPROPERTY(EditAnywhere, Category = "Area Info")
		float ValidDis = 300.f;
	UPROPERTY(EditAnywhere, Category = "Area Info")
		float SearchAreaRadius = 2000.f;
	UPROPERTY(EditAnywhere, Category = "Area Info")
		float RespawnTime = 10.f;

	
private:

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		ACPP_Character* Target;

	UPROPERTY(EditAnywhere, Category = "Area Info")
		TArray<AEnemyBase*> Enemys;

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		TArray<TSubclassOf<AEnemyBase>> EnemyClass;

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		TArray<AActor*> SpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "Area Info")
		int32 EnemysNum;

	FVector CenterPos;
	
};
