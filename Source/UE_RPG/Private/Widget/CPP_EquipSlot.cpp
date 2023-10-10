// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_EquipSlot.h"
#include "CPP_Character.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widget/TootipWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Widget/SlotDrag.h"
#include "Widget/CPP_DragSlotWidget.h"
#include "Widget/CPP_Slot.h"
#include "Item/Item.h"
#include "Item/Weapon.h"
#include "Item/Gun/Rifle.h"



void UCPP_EquipSlot::NativeConstruct()
{
	Super::NativeConstruct();
	DefaultBorderColor = SlotBorder->GetBrushColor(); 
	InactiveSlot();
}

void UCPP_EquipSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!IsValid(DragWidgetClass))
		return;

	UCPP_DragSlotWidget* dragWidget = CreateWidget<UCPP_DragSlotWidget>(GetWorld(), DragWidgetClass);
	if (IsValid(dragWidget))
		dragWidget->UpdataWidget(ItemRef);

	USlotDrag* dragSlot = Cast<USlotDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDrag::StaticClass()));

	if (IsValid(dragSlot))
	{
		dragSlot->bFromEquipmentSlot = true;
		dragSlot->DefaultDragVisual = dragWidget;
		dragSlot->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = dragSlot;
}

bool UCPP_EquipSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);

	if (IsValid(dragSlot) 
		&& dragSlot->WidgetRef
		&& dragSlot->WidgetRef->GetItemRef()->ItemType == EItemCategory::EIS_Equipment)
	{
		dragSlot->WidgetRef->EquipSlotItem();
		return true;
	}

	return false;
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

	if (ItemIcon->GetIsEnabled() && ItemRef)
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			InventoryRef->AddItem(ItemRef);
			UnEquipWeapon();
		}
		FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return reply.NativeReply;
	}
	return FReply::Handled();
}

void UCPP_EquipSlot::UpdateEquipmentSlot(UItem* weapon)
{
	ItemRef = weapon;
	ActiveSlot();
	SetSlotToolTip();
}

void UCPP_EquipSlot::UnEquipWeapon()
{
	InactiveSlot();
	AWeapon* weapon = PlayerRef->GetEquippedWeapon();

	if (IsValid(weapon))
	{
		weapon->SetActiveWeapon(false);
		PlayerRef->SetStateUnEquipped();
	}
}




