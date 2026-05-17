// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "Item/ItemData.h"
#include "Item/Weapon/EquipmentData.h"
#include "CPP_SlotBase.generated.h"

class UCPP_DragSlotWidget;
class UBorder;
class UImage;
class UTootipWidget;
class UDragDropOperation;

struct FGeometry;
struct FPointerEvent;


DECLARE_DELEGATE_FourParams(FOnDragDetectedDelegate, const FGeometry&, const FPointerEvent&, UDragDropOperation*&, const int32);
DECLARE_DELEGATE_RetVal_FourParams(bool, FOnDropDelegate, const FGeometry&, const FDragDropEvent&, UDragDropOperation*, const int32);
DECLARE_DELEGATE_ThreeParams(FOnMouseButtonDownDelegate, const FGeometry&, const FPointerEvent&, const int32);
DECLARE_DELEGATE_RetVal_ThreeParams(FReply, FOnSlotMouseButtonDoubleClickDelegate, const FGeometry&, const FPointerEvent&, const int32);

UCLASS()
class UE_RPG_API UCPP_SlotBase : public UCustomUMGWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* SlotBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Component")
	TSubclassOf<UTootipWidget> TootipWidgetClass;

	FOnDragDetectedDelegate OnSlotDragDetected;
	FOnDropDelegate OnSlotDrop;
	FOnMouseButtonDownDelegate OnSlotMouseButtonDown;
	FOnSlotMouseButtonDoubleClickDelegate OnSlotMouseButtonDoubleClick;

	/**for drag over event only once*/
	bool bDraggedOver = false;

	void SetBorderDefault();
	void SetBorderWhite();
	
protected:

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void InactiveSlot();
	virtual void ActiveSlot(UTexture2D* icon);
	void SetSlotToolTip(const FItemInfoTable* itemInfo);
	void SetSlotToolTip(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo);

	virtual	bool IsValidDragOverTarget(UDragDropOperation* InOperation) { return true; }

protected:

	/**tooltip*/
	UPROPERTY()
	UTootipWidget* ToolTip = nullptr;

	/**slot info*/
	FLinearColor DefaultBorderColor;

private:

};
