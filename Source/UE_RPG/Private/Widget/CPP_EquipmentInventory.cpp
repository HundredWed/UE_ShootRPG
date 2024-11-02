// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_EquipmentInventory.h"
#include "Widget/CPP_EquipSlot.h"
#include "Components/TextBlock.h"
#include "Item/Item.h"

bool UCPP_EquipmentInventory::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	return true;
}

void UCPP_EquipmentInventory::UpdateEquipSlot(UItem* item)
{
	EquipSlot->UpdateEquipmentSlot(item);

	const FString String = FString::Printf(TEXT("%d"), item->ATK);
	const FText text = FText::FromString(String);
	ATKText->SetText(text);
}
