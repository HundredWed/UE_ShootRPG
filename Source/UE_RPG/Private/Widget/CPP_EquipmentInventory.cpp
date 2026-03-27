// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_EquipmentInventory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Widget/CPP_EquipSlot.h"
#include "Components/TextBlock.h"
#include "Inventory.h"
#include "Widget/CPP_DragSlotWidget.h"
#include "Widget/SlotDrag.h"

void UCPP_EquipmentInventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (EquipSlot)
	{
		EquipSlot->SetEquipmentType(EEquipmentType::Weapon);
		EquipSlots.Add(EEquipmentType::Weapon, EquipSlot);

		EquipSlot->OnEquipDragDetected.BindUObject(this, &UCPP_EquipmentInventory::OnSlotDragDetected);
		EquipSlot->OnEquipDrop.BindUObject(this, &UCPP_EquipmentInventory::OnSlotDrop);
		EquipSlot->OnEquipMouseButtonDown.BindUObject(this, &UCPP_EquipmentInventory::OnSlotMouseButtonDown);
	}
		
	//if (OffensiveRingSlot) EquipSlots.Add(EEquipmentType::OffensiveRing, OffensiveRingSlot);
	//if (DefensiveRingSlot) EquipSlots.Add(EEquipmentType::DefensiveRing, DefensiveRingSlot);
}

bool UCPP_EquipmentInventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}

void UCPP_EquipmentInventory::OnSlotDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation, const FName& equipmentID)
{
	if (!IsValid(DragWidgetClass) || !InventoryRef.IsValid())
		return;

	const FItemInfoTable* itemData = InventoryRef->RequestItemData(equipmentID);
	const FEquipmentInfoTable* equipmentData = InventoryRef->RequestEquipmentData(equipmentID);
	UCPP_DragSlotWidget* dragWidget = CreateWidget<UCPP_DragSlotWidget>(GetWorld(), DragWidgetClass);
	if (IsValid(dragWidget) && itemData)
		dragWidget->UpdataWidget(itemData, 1);

	USlotDrag* dragSlot = Cast<USlotDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDrag::StaticClass()));

	if (IsValid(dragSlot))
	{
		dragSlot->bFromEquipmentSlot = true;
		dragSlot->EquipmentType = equipmentData->EquipmentType;
		dragSlot->DefaultDragVisual = dragWidget;
		dragSlot->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = dragSlot;
}

bool UCPP_EquipmentInventory::OnSlotDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation, const FName& equipmentID)
{
	USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);

	if (IsValid(dragSlot) && InventoryRef.IsValid())
	{
		InventoryRef->SetEquipWeapon(dragSlot->GetIndex());
		return true;
	}

	return false;
}

FReply UCPP_EquipmentInventory::OnSlotMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const FName& equipmentID)
{
	if (InventoryRef.IsValid())
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			InventoryRef->AddItem(equipmentID);
			InventoryRef->RequestTakeOffWeapon();
		}
		FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return reply.NativeReply;
	}
	return FReply::Handled();
}

void UCPP_EquipmentInventory::UpdateEquipSlot(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo)
{
	EquipSlot->UpdateEquipmentSlot(itemInfo, equipmentInfo);

	const FString String = FString::Printf(TEXT("%d"), equipmentInfo->ATK);
	const FText text = FText::FromString(String);

	

	ATKText->SetText(text);
}

FName UCPP_EquipmentInventory::GetEquipmentID(EEquipmentType equipmentType)
{
	if (UCPP_EquipSlot** FoundSlot = EquipSlots.Find(equipmentType))
	{
		return (*FoundSlot)->GetEquipmentID();
	}

	return NAME_None;
}

void UCPP_EquipmentInventory::InitEquipmentInventory(TWeakObjectPtr<UInventory> inventory, TSubclassOf<UCPP_DragSlotWidget> dragWidgetClass)
{
	InventoryRef = inventory;
	DragWidgetClass = dragWidgetClass;
}
