// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CPP_SlotBase.h"
#include "CPP_EquipSlot.generated.h"

DECLARE_DELEGATE_FourParams(FOnEquipDragDetectedDelegate, const FGeometry&, const FPointerEvent&, UDragDropOperation*&, const FName&);
DECLARE_DELEGATE_RetVal_FourParams(bool, FOnEquipDropDelegate, const FGeometry&, const FDragDropEvent&, UDragDropOperation*, const FName&);
DECLARE_DELEGATE_ThreeParams(FOnEquipMouseButtonDownDelegate, const FGeometry&, const FPointerEvent&, const FName&);

UCLASS()
class UE_RPG_API UCPP_EquipSlot : public UCPP_SlotBase
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	/**from UCPP_SlotBase*/
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	void UpdateEquipmentSlot(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo);
	void TakeOff();
	FName GetEquipmentID() { return EquipmentID; }
	void SetEquipmentType(EEquipmentType type) { MyEquipmentType = type; }


	FOnEquipDragDetectedDelegate OnEquipDragDetected;
	FOnEquipDropDelegate OnEquipDrop;
	FOnEquipMouseButtonDownDelegate OnEquipMouseButtonDown;

private:

	friend class UCPP_EquipmentInventory;

	FName EquipmentID = NAME_None;
	EEquipmentType MyEquipmentType;
};
