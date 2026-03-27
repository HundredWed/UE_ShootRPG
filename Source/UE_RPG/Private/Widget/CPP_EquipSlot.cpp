// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_EquipSlot.h"
#include "CPP_Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/TootipWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Widget/SlotDrag.h"
#include "Widget/CPP_Slot.h"



void UCPP_EquipSlot::NativeConstruct()
{
	Super::NativeConstruct();
	DefaultBorderColor = SlotBorder->GetBrushColor(); 
	InactiveSlot();
}

void UCPP_EquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	OnEquipDragDetected.Execute(InGeometry, InMouseEvent, OutOperation, EquipmentID);
}

bool UCPP_EquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	return OnEquipDrop.Execute(InGeometry, InDragDropEvent, InOperation, EquipmentID);
}

bool UCPP_EquipSlot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry,  InDragDropEvent, InOperation);
}

void UCPP_EquipSlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

FReply UCPP_EquipSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (ItemIcon->GetIsEnabled())
	{
		return OnEquipMouseButtonDown.Execute(InGeometry, InMouseEvent, EquipmentID);
	}		

	return FReply::Unhandled();
}

void UCPP_EquipSlot::UpdateEquipmentSlot(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo)
{
	ActiveSlot(itemInfo->IconTexture);
	SetSlotToolTip(itemInfo, equipmentInfo);
}

void UCPP_EquipSlot::TakeOffWeapon()
{
	InactiveSlot();
}




