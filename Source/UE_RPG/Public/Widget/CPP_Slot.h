// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CPP_SlotBase.h"
#include "CPP_Slot.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE_OneParam(FOnCombineDelegate, const int32);

UCLASS()
class UE_RPG_API UCPP_Slot : public UCPP_SlotBase
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CombineButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget)) 
	UTextBlock* TextAmount;


	int32 MyIndex = 0;
	int32 MyAmount = 0;
	bool  bMyItemCanStacked = false;

	/**found CombinableSlot*/
	int32 CombinableSlot = -1;
	bool bActiveCombineButton = false;

	/**CombinableSlot*/
	int32 LinkedCombinableSlot = -1;

	FOnCombineDelegate OnCombineDelegate;

public: 
	virtual void InactiveSlot() override;
	virtual void ActiveSlot(UTexture2D* icon) override;

	void UpdateSlot(const FItemInfoTable* itemData, const int32 index, const int32 amount);
	void UpdateSlot(const FItemInfoTable* itemData, const FEquipmentInfoTable* equipmentData, const int32 index);
	

	/**Combine function*/
	void InactiveCombinableSlot();
	void ActiveCombinableSlot();
	bool GetIsActiveCombineButton() { return bActiveCombineButton; }
	
	UFUNCTION()
	void CombineItem();

	void SetBorder();

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
};
