// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "UE_RPG/UtilityMecro.h"
#include "CharacterStates.h"
#include "CPP_PlayerStateBar.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_PlayerStateBar : public UCustomUMGWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerLevel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HealthAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PlayerHealth;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* BackGroundBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PlayerMana;

	
	void InitStateBar(const FCharacterStats& stat);
	void UpdateHealthBarPercent(const float currentHp, const float max);
	void UpdateManaBarPercent(const float percent);
	//void SetStaminaBarPercent(float Percent);
	void UpdateLevel(int32 level);
	void UpdateHealthbarText(const int32 current, const int32 max);

	void UpdateBackGroundBar();
	void SetPercentTick();
	bool CheckPercent();
	void StartManaRecover(const float percent);

private:

	/**BackGround*/
	float BGPercent;
	float CurrentBGPercent;
	bool UpdatePercent = false;
};
