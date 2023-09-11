// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_InventoryWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* CloseButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UUniformGridPanel* SlotPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USetAmountWidget* SpliteWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UScrollBox* InventoryScollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
		TSubclassOf< class UCPP_Slot> SlotWidgetClass;

	/**the value for render transform SpliteWidget*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
		int32 SlotBoxSize = 64;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Widget")
		float InventoryBoxSize = 500;

	UPROPERTY()
		TArray<class UCPP_Slot*> SlotWidgetArray;

	

public:	

	UFUNCTION()
		void GenerateSlotWidget(const uint8 slotsParRow);

	UFUNCTION()
		void CloseWidget();

	void SetPanelEnabled(bool enabled);
	void SetSpliteWidget(class UCPP_Slot* fromSlot, class UCPP_Slot* toSlot);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

private:

	UPROPERTY()
		class UCPP_Slot* SlotWidget;

};
