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

	TArray<EEquipmentType> typeArray;

	for (uint8 i = 0; i < (uint8)EEquipmentType::EquipmentTypeCount; ++i)
	{
		typeArray.Add(static_cast<EEquipmentType>(i));
	}

	for (EEquipmentType type : typeArray)
	{
		if (UCPP_EquipSlot* slot = GetEquipSlot(type))
		{
			slot->SetEquipmentType(type);
			slot->OnEquipDragDetected.BindUObject(this, &UCPP_EquipmentInventory::OnSlotDragDetected);
			slot->OnEquipDrop.BindUObject(this, &UCPP_EquipmentInventory::OnSlotDrop);
			slot->OnEquipMouseButtonDown.BindUObject(this, &UCPP_EquipmentInventory::OnSlotMouseButtonDown);

			EquipSlots.Add(type, slot);
		}
	}
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
		dragSlot->EquipmentType = equipmentData->EquipmentStat.EquipmentType;
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

void UCPP_EquipmentInventory::OnSlotMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const FName& equipmentID)
{
	if (InventoryRef.IsValid())
	{
		if (const FEquipmentInfoTable* equipmentData = InventoryRef->RequestEquipmentData(equipmentID))
		{
			InventoryRef->AddItem(equipmentID, 1, false);
			InventoryRef->RequestTakeOffEquipment(equipmentData->EquipmentStat.EquipmentType);
			SetTotalState(equipmentData, true);
		}		
	}
}

void UCPP_EquipmentInventory::SetTotalState(const FEquipmentInfoTable* equipmentInfo, bool isSubtract)
{
	if (isSubtract)
	{
		TotalATK -= equipmentInfo->EquipmentStat.ATK;
		TotalDEF -= equipmentInfo->EquipmentStat.DEF;
		TotalManaDensity -= equipmentInfo->EquipmentStat.ManaDensity;
	}
	else
	{
		TotalATK += equipmentInfo->EquipmentStat.ATK;
		TotalDEF += equipmentInfo->EquipmentStat.DEF;
		TotalManaDensity += equipmentInfo->EquipmentStat.ManaDensity;
	}
	
	const FString stringATK = FString::Printf(TEXT("%d"), TotalATK);
	const FText textATK = FText::FromString(stringATK);
	const FString stringDEF = FString::Printf(TEXT("%d"), TotalDEF);
	const FText textDEF = FText::FromString(stringDEF);
	const FString stringManaDensity = FString::Printf(TEXT("%d"), TotalManaDensity);
	const FText textManaDensity = FText::FromString(stringManaDensity);


	ATKText->SetText(textATK);
	DEFText->SetText(textDEF);
	ManaDensityText->SetText(textManaDensity);
}

UCPP_EquipSlot* UCPP_EquipmentInventory::GetEquipSlot(EEquipmentType equipmentType)
{
	switch (equipmentType)
	{
	case EEquipmentType::None:
		break;
	case EEquipmentType::Weapon:
		return WeaponSlot;
	case EEquipmentType::OffensiveRing:
		return OffensiveRingSlot;
	case EEquipmentType::DefensiveRing:
		return DefensiveRingSlot;
	default:
		break;
	}

	return nullptr;
}

void UCPP_EquipmentInventory::UpdateEquipSlot(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo)
{
	if (TObjectPtr<UCPP_EquipSlot>* slot = EquipSlots.Find(equipmentInfo->EquipmentStat.EquipmentType))
	{
		//만약 전에 장착한 무기가 있다면 해당 무기만큼 능력치를 뺀 후 업데이트
		if ((*slot)->GetEquipmentID().IsNone() == false)
		{
			if (const FEquipmentInfoTable* prevEquipment = InventoryRef->RequestEquipmentData((*slot)->GetEquipmentID()))
			{
				SetTotalState(prevEquipment, true);				
			}
		}	
		
		(*slot)->UpdateEquipmentSlot(itemInfo, equipmentInfo);
		SetTotalState(equipmentInfo);		
	}
}

FName UCPP_EquipmentInventory::GetEquipmentID(EEquipmentType equipmentType)
{
	if (TObjectPtr<UCPP_EquipSlot>* FoundSlot = EquipSlots.Find(equipmentType))
	{
		return (*FoundSlot)->GetEquipmentID();
	}

	return NAME_None;
}

void UCPP_EquipmentInventory::InitEquipmentInventory(TWeakObjectPtr<UInventory> inventory, TSubclassOf<UCPP_DragSlotWidget> dragWidgetClass)
{
	if (!inventory.IsValid()) return;

	InventoryRef = inventory;
	DragWidgetClass = dragWidgetClass;

	TMap<EEquipmentType, FEquipmentSlot> slots = InventoryRef->GetEquipmentSlots();

	for (auto slot : slots)
	{
		if (!slot.Value.EquipmentID.IsNone())
		{
			if (TObjectPtr<UCPP_EquipSlot>* foundSlot = EquipSlots.Find(slot.Key))
			{
				const FName& id = slot.Value.EquipmentID;

				(*foundSlot)->EquipmentID = id;
				const FItemInfoTable* itemData = InventoryRef->RequestItemData(id);
				const FEquipmentInfoTable* equipmentData = InventoryRef->RequestEquipmentData(id);
				(*foundSlot)->UpdateEquipmentSlot(itemData, equipmentData);

				TotalATK = 0;
				TotalDEF = 0;
				TotalManaDensity = 0.f;
				SetTotalState(equipmentData);
			}
		}
		
	}
}

void UCPP_EquipmentInventory::TakeOffEquipment(EEquipmentType equipmentType)
{
	if (TObjectPtr<UCPP_EquipSlot>* FoundSlot = EquipSlots.Find(equipmentType))
	{
		return (*FoundSlot)->TakeOff();
	}
}
