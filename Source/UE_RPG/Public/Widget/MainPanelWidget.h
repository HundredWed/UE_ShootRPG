// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CharacterStates.h"
#include "MainPanelWidget.generated.h"

class UCPP_InventoryWidget;
class USetAmountWidget;
class UCPP_PlayerStateBar;
class UCPP_InProgressQuestsWidget;
class UCPP_StatComponent;
class UImage;
class UWidgetAnimation;
class ACPP_Character;
class UCPP_SettingsWidget;

UCLASS()
class UE_RPG_API UMainPanelWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	FORCEINLINE UCPP_InventoryWidget* GetInventoryWidget() { return InventoryWidget; }
	FORCEINLINE UCPP_PlayerStateBar* GetStateWidget() { return StateWidget; }
	
	void ShowInteractWidget();
	void HideInteractWidget();

	UFUNCTION(BlueprintImplementableEvent)
	bool UpdatePopupText(const FText& text);

	UFUNCTION(BlueprintImplementableEvent)
	bool ManaOverHeat();
	UFUNCTION(BlueprintImplementableEvent)
	bool ManaCoolDown();


	void BindCharacter(ACPP_Character* player);

	void InitState(const FCharacterStats& stat);
	

	//void SetStaminaBarPercent(float Percent);
	void UpdateLevel(int32 level);

	void PlayFadeIn();
	void PlayFadeOut();

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override ;

private:
	void UpdateHealthBarPercent(const float currentHp, const float max);
	void UpdateManaBarPercent(const float currentMP, const float max);
	void RecoverManaBar(const float percent);

	void ManaOverHeatEvent();
	void ManaCoolDownEvent();


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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UUserWidget* InteractWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* BackScreen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_SettingsWidget* SettingsWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* FadeAnim;

	bool IsPrevBackScreenEvent = true;
};
