// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "MainPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UMainPanelWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCPP_InventoryWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USetAmountWidget* ThrowWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCPP_PlayerStateBar* StateWidget;

	FORCEINLINE UCPP_InventoryWidget* GetInventoryWidget() { return InventoryWidget; }
	FORCEINLINE UCPP_PlayerStateBar* GetStateWidget() { return StateWidget; }

	/**StateWidget*/
	void InitState(const int32 level, const float healt, const float maxHP, const float mana, const float stamina);
	void UpdateHealthBarPercent(const float currentHp, const float max);
	void UpdateManaBarPercent(const float percent);
	//void SetStaminaBarPercent(float Percent);
	void UpdateLevel(int32 level);

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override ;
};
