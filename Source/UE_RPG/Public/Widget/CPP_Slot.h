// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "Inventory.h"
#include "CPP_Slot.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_Slot : public UCustomUMGWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UBorder* SlotBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* CombineButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Compoenet")
		TSubclassOf< class UCPP_DragSlotWidget> DragWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Compoenet")
		TSubclassOf< class UTootipWidget> TootipWidgetClass;


	UPROPERTY()
		class UTootipWidget* toolTip;

	int16 MyArrayNumber = 0;
	int32 MyAmount = 0;
	bool  bMyItemCanStacked = false;

	/**CombinableSlot*/
	int16 LinkedCombinableSlot = -1;

	/**for drag over event only once*/
	bool bDraggedOver = false;

public: 
	
	void UpdateSlot(const int16 index);
	void OnUseItem();
	class UItem* GetItemItemInfo() { return ItemRef; }

	/**Combinae function*/
	void InactiveCombinableSlot();
	void ActiveCombinableSlot();
	void CheckCombinability(const int16 fromIndex);
	bool GetIsActiveCombineButton() { return bActiveCombineButton; }
	
	UFUNCTION()
		void CombineItem();

protected:

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:

	void InactiveSlot();
	void ActiveSlot();
	void InitSlotInfo();
	void SetSlotToolTip();

	void SearchCombinableSlot();

	/**item info*/
	UPROPERTY()
		class UItem* ItemRef;

	UPROPERTY()
		class UCPP_DragSlotWidget* DragSlotWidget;


	/**slot info*/
	FInventorySlot InventorySlotinfo;
	FLinearColor DefaultBorderColor;

	/**found CombinableSlot*/
	int16 CombinableSlot = -1;
	bool bActiveCombineButton = false;

};
