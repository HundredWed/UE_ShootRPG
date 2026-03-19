// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CharacterStates.h"
#include "MainPanelWidget.generated.h"

class ACPP_Character;
class UCPP_InventoryWidget;
class USetAmountWidget;
class UCPP_PlayerStateBar;
class UCPP_InProgressQuestsWidget;

UCLASS()
class UE_RPG_API UMainPanelWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	FORCEINLINE UCPP_InventoryWidget* GetInventoryWidget() { return InventoryWidget; }
	FORCEINLINE UCPP_PlayerStateBar* GetStateWidget() { return StateWidget; }

	UFUNCTION(BlueprintImplementableEvent)
	bool UpdatePopupText(const FText& text);


	/**StateWidget*/
	void BindCharacterStat(ACPP_Character* player);

	void InitState(const FCharacterStats& stat);
	

	//void SetStaminaBarPercent(float Percent);
	void UpdateLevel(int32 level);

	bool ToggleQuestList();

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override ;

private:
	void UpdateHealthBarPercent(const float currentHp, const float max);
	void UpdateManaBarPercent(const float percent);


	bool IsQuestListActivate = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_InventoryWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	USetAmountWidget* ThrowWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_PlayerStateBar* StateWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_InProgressQuestsWidget* InProgressQuestsWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UUserWidget* PopupWidget;
};
