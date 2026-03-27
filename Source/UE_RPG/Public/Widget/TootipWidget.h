// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "Item/ItemData.h"
#include "Item/Weapon/EquipmentData.h"
#include "TootipWidget.generated.h"

class UImage;
class UTextBlock;
class USizeBox;


UCLASS()
class UE_RPG_API UTootipWidget : public UCustomUMGWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Category;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ATKText;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DEFText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ManaCostText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ManaDensityText;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USizeBox* BlankSpace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Price;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Weight;
	

	void InitToolTip(const FItemInfoTable* itemInfo);
	void InitToolTip(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo);

private:

	void SetItemType(const FItemInfoTable* itemInfo);

};
