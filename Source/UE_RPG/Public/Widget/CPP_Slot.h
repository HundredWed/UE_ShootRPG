// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CPP_SlotBase.h"
#include "Inventory.h"
#include "CPP_Slot.generated.h"

/**
 * Inevntory Slot
 */
UCLASS()
class UE_RPG_API UCPP_Slot : public UCPP_SlotBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* CombineButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget)) 
		class UTextBlock* TextAmount;


	int16 MyArrayNumber = 0;
	int32 MyAmount = 0;
	bool  bMyItemCanStacked = false;

	/**CombinableSlot*/
	int16 LinkedCombinableSlot = -1;

public: 
	virtual void InactiveSlot() override;
	virtual void ActiveSlot() override;

	void UpdateSlot(const int16 index);
	void OnUseItem();
	void EquipSlotItem();
	class UItem* GetItemInfoFromSlot() { return ItemRef; }
	

	/**Combinae function*/
	void InactiveCombinableSlot();
	void ActiveCombinableSlot();
	void CheckCombinability(const int16 fromIndex);
	bool GetIsActiveCombineButton() { return bActiveCombineButton; }
	
	UFUNCTION()
		void CombineItem();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	/**from UCPP_SlotBase*/
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:

	

	/**you must init ItemRef this Func*/
	 void InitSlotInfo();

	void SearchCombinableSlot();

	UPROPERTY()
		class UCPP_DragSlotWidget* DragSlotWidget;

	/**slot info*/
	FInventorySlot InventorySlotinfo;

	/**found CombinableSlot*/
	int16 CombinableSlot = -1;
	bool bActiveCombineButton = false;

};
