// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_EquipSlot.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_EquipSlot : public UCustomUMGWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UBorder* SlotBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Compoenet")
		TSubclassOf< class UCPP_DragSlotWidget> DragWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Compoenet")
		TSubclassOf< class UTootipWidget> TootipWidgetClass;

	UPROPERTY()
		class UTootipWidget* ToolTip;

	/**for drag over event only once*/
	bool bDraggedOver = false;

protected:

	virtual void NativeConstruct() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	void InitEquipSlot();
	void UpdateEquipSlot(class UItem* item);
	void SetSlotToolTip(class UItem* item);

private:

	/**slot info*/
	FLinearColor DefaultBorderColor;

};
