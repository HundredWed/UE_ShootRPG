// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "Item/ItemData.h"
#include "Item/Weapon/EquipmentData.h"
#include "CPP_InventoryWidget.generated.h"

class UInventory;
class UCPP_Slot;
class UCPP_DragSlotWidget;
class UButton;
class UUniformGridPanel;
class USetAmountWidget;
class UCPP_EquipmentInventory;
class UScrollBox;
class UTextBlock;

UCLASS()
class UE_RPG_API UCPP_InventoryWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SortButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* SlotPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	USetAmountWidget* SplitWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCPP_EquipmentInventory* EquipmentInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* InventoryScrollBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* WeightText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
	TSubclassOf<UCPP_Slot> SlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
	TSubclassOf<UCPP_DragSlotWidget> DragWidgetClass;

	/**the value for render transform SplitWidget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
	int32 SlotBoxSize = 64;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
	float InventoryBoxSize = 500;

	UPROPERTY()
	TArray<UCPP_Slot*> SlotWidgetArray;

public:	

	UFUNCTION()
	void GenerateSlotWidget(UInventory* inventory, const int32 slotsParRow);

	UFUNCTION()
	void SortInventory();

	void SetPanelEnabled(bool enabled);
	void SetSplitWidget(const int32 fromSlotIndex, const int32 toSlotIndex);
	void UpdateWeightText(const float amount);
	void UpdateWeightMaxAmount(const float amount);
	void UpdateGoldText(const int32 amount);
	void UpdateSlot(const FItemInfoTable* itemData, const int32 index, const int32 amount);
	void UpdateSlot(const FItemInfoTable* itemData, const FEquipmentInfoTable* equipmentData, const int32 index);
	void CheckCombinability(const int32 toIndex, const int32 fromIndex);

	FName GetCurrentEquipmentID(EEquipmentType equipmentType);
	UCPP_Slot* GetSlotWidget(const int32 index);
	void UpdateEquipmentInventory(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	UFUNCTION()
	virtual void CloseWidget() override;

private:

	void SeSlotInfo(UCPP_Slot* slot, const int32 index );

	//일반 슬롯
	void OnSlotDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation, const int32 index);
	bool OnSlotDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation, const int32 index);
	void OnSlotMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const int32 index);
	FReply OnSlotMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const int32 index);


	UFUNCTION()
	void ChangeItem(const int32 index);

	void OnUseItem(const int32 index);
	void EquipSlotItem(const int32 fromIndex);
	void TakeOffEquipment(EEquipmentType equipmentType, const int32 index);
	void SearchCombinableSlot(EItemCategory itemType, const int32 startIndex);

	TWeakObjectPtr<UInventory> InventoryRef;

	float MaxWeight = 0.0f;

};
