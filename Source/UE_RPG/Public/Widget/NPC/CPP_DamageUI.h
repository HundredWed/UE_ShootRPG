// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ST_DamageFeedback.h"
#include "CPP_DamageUI.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class UE_RPG_API UCPP_DamageUI : public UUserWidget
{
	GENERATED_BODY()

public:	
		
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* DamageAnimRight;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* DamageAnimLeft;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* DamageAnimCritical;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* DamageAnimImmune;
	
	UFUNCTION(BlueprintImplementableEvent)
	bool SetTextType(EDamageType type);

	bool bActivate = false;

	void UpdateWidget(const float amount, EDamageType type);

private:

	UFUNCTION(BlueprintCallable)
	void EndAnim();

	void NormalEvent();
	void CriticalEvent();
	void WeakPointEvent();
	void WeakPointCritEvent();
	void ImmuneEvent();
};
