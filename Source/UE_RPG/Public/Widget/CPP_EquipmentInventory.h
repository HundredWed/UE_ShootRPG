// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "Item/Weapon/EquipmentData.h"
#include "CPP_EquipmentInventory.generated.h"

class UCPP_EquipSlot;
class UTextBlock;
class UCPP_DragSlotWidget;
class UInventory;

struct FItemInfoTable;

UCLASS()
class UE_RPG_API UCPP_EquipmentInventory : public UCustomUMGWidget
{
	GENERATED_BODY()

public:

	void UpdateEquipSlot(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo);
	FName GetEquipmentID(EEquipmentType equipmentType);
	void InitEquipmentInventory(TWeakObjectPtr<UInventory> inventory, TSubclassOf<UCPP_DragSlotWidget> dragWidgetClass);
	
	void TakeOffEquipment(EEquipmentType equipmentType);

protected:

	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

private:

	void OnSlotDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation, const FName& equipmentID);
	bool OnSlotDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation, const FName& equipmentID);
	void OnSlotMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, const FName& equipmentID);

	void SetTotalState(const FEquipmentInfoTable* equipmentInfo, bool isSubtract = false);

	UCPP_EquipSlot* GetEquipSlot(EEquipmentType equipmentType);
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_EquipSlot* WeaponSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_EquipSlot* OffensiveRingSlot;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCPP_EquipSlot* DefensiveRingSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ATKText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* DEFText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ManaDensityText;


	UPROPERTY()
	TSubclassOf<UCPP_DragSlotWidget> DragWidgetClass;

	UPROPERTY()
	TMap<EEquipmentType, TObjectPtr<UCPP_EquipSlot>> EquipSlots;

	TWeakObjectPtr<UInventory> InventoryRef;

	int32 TotalATK = 0;
	int32 TotalDEF = 0;
	float TotalManaDensity = 0;
};
