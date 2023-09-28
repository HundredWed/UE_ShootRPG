// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_SlotBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UCPP_SlotBase : public UCustomUMGWidget
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
	
protected:

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void InactiveSlot();
	virtual void ActiveSlot();
	void SetSlotToolTip();

public:

	FORCEINLINE class UItem* GetItemRef() { return ItemRef; }

protected:

	/**tooltip*/
	UPROPERTY()
		class UTootipWidget* ToolTip = nullptr;

	/**item info*/
	UPROPERTY()
		class UItem* ItemRef = nullptr;

	/**slot info*/
	FLinearColor DefaultBorderColor;

	/**for drag over event only once*/
	bool bDraggedOver = false;
private:

};
