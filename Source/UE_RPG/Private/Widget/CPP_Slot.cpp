// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CPP_Character.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Item/ItemAbility.h"
#include "Widget/CPP_DragSlotWidget.h"
#include "Widget/SetAmountWidget.h"
#include "Components/Border.h"
#include "Widget/SlotDrag.h"
#include "Components/Image.h"
#include "Item/Item.h"
#include "Widget/TootipWidget.h"
#include "Widget/CPP_EquipmentInventory.h"
#include "Widget/CPP_EquipSlot.h"

void UCPP_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	CombineButton->OnPressed.AddDynamic(this, &UCPP_Slot::CombineItem);
	DefaultBorderColor = SlotBorder->GetBrushColor();
	ItemRef = nullptr;
}

void UCPP_Slot::UpdateSlot(const int16 index)
{
	if (IsValid(InventoryRef))
	{
		MyArrayNumber = index;
		bool isSlotEmpty = InventoryRef->IsSlotEmpty(index);

		if (isSlotEmpty)
		{
			InactiveSlot();			
		}
		else
		{
			InitSlotInfo();
			ActiveSlot();

			if (ItemRef->bCanStacked)
			{
				TextAmount->SetText(FText::Format(NSLOCTEXT("CPP_Slot", "TextAmount", "x{0}"), MyAmount));
				TextAmount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				TextAmount->SetVisibility(ESlateVisibility::Hidden);
			}

			/**set tooltip*/
			SetSlotToolTip();

			/**FindCombinableSlot*/
			SearchCombinableSlot();
		}
	}
}


void UCPP_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!IsValid(DragWidgetClass))
		return;

	UCPP_DragSlotWidget* dragWidget = CreateWidget<UCPP_DragSlotWidget>(GetWorld(), DragWidgetClass);
	if(IsValid(dragWidget))
		dragWidget->UpdataWidget(ItemRef, MyAmount);

	USlotDrag* dragSlot = Cast<USlotDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDrag::StaticClass()));

	if (IsValid(dragSlot))
	{
		if (LinkedCombinableSlot != -1)
		{
			InventoryRef->InventoryWidget->SlotWidgetArray[LinkedCombinableSlot]->InactiveCombinableSlot();
		}

		dragSlot->WidgetRef = this;
		dragSlot->DefaultDragVisual = dragWidget;
		dragSlot->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = dragSlot;
}

bool UCPP_Slot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

void UCPP_Slot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool UCPP_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);

	if (dragSlot)
	{
		if (dragSlot->WidgetRef && dragSlot->WidgetRef != this)
		{
			const int16 fromIndex = dragSlot->WidgetRef->MyArrayNumber;
			const int16 toIndex = MyArrayNumber;

			bDraggedOver = false;
			//border
			SlotBorder->SetBrushColor(DefaultBorderColor);

			if (InventoryRef->CanAddToIndex(fromIndex, toIndex))
			{
				InventoryRef->AddToIndex(fromIndex, toIndex);
				return true;
			}
			else
			{
				if (PlayerRef->bShiftDown)
				{
					InventoryRef->InventoryWidget->SetSpliteWidget(dragSlot->WidgetRef, this);
					return true;
				}
				else
				{
					InventoryRef->SwapSlot(fromIndex, toIndex);
					return true;
				}
			}
		}
		else if(dragSlot->bFromEquipmentSlot)
		{
			InventoryRef->UnEquipWeaponAndAddItem(MyArrayNumber);
			return true;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

FReply UCPP_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (ItemIcon->GetIsEnabled())
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			if (IsValid(InventoryRef) 
				&& ItemRef->ItemType != EItemCategory::EIS_Equipment)
			{
				OnUseItem();
			}
			else if(IsValid(InventoryRef)
				&& ItemRef->ItemType == EItemCategory::EIS_Equipment)
			{
				EquipSlotItem();
			}
		}
		else
		{
			FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			return reply.NativeReply;
		}
		
	}

	return FReply::Handled();
}

FReply UCPP_Slot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		//icon
		if (ItemIcon->GetIsEnabled())
		{
			if (IsValid(InventoryRef) && ItemRef->ItemType != EItemCategory::EIS_Equipment)
			{
				OnUseItem();
			}
			else
			{
				return FReply::Handled();
			}
		}
	}
	return FReply::Handled();
}



void UCPP_Slot::OnUseItem()
{
	

	AActor* abilityActor = InventoryRef->GetAbilityActor(InventorySlotinfo.Item->ItemInfoID);
	if (abilityActor)
	{
		IItemAbility* itemAbility = Cast<IItemAbility>(abilityActor);
		if (itemAbility)
		{
			const uint32 value = InventoryRef->SlotsArray[MyArrayNumber].Item->ConsumeValue;
			itemAbility->UseItem(PlayerRef, value);
			InventoryRef->RemoveItemAtIndex(MyArrayNumber, 1);

			GEngine->AddOnScreenDebugMessage(
				INDEX_NONE,
				30.f,
				FColor::Blue,
				FString::Printf(TEXT("get from Tmap")));
		}
	}
	else
	{
		TSubclassOf<AActor> itemClass = InventoryRef->SlotsArray[MyArrayNumber].Item->ItemClass;
		if (IsValid(itemClass))
		{
			AActor* itemActor = GetWorld()->SpawnActor(InventoryRef->SlotsArray[MyArrayNumber].Item->ItemClass);

			IItemAbility* itemAbility = Cast<IItemAbility>(itemActor);
			if (itemAbility)
			{
				const uint32 value = InventoryRef->SlotsArray[MyArrayNumber].Item->ConsumeValue;
				itemAbility->UseItem(PlayerRef, value);

				InventoryRef->AddItemManage(InventorySlotinfo.Item->ItemInfoID, itemActor);
				InventoryRef->RemoveItemAtIndex(MyArrayNumber, 1);
				InventoryRef->StartAbilityActorLife(InventorySlotinfo.Item->ItemInfoID);
			}

			GEngine->AddOnScreenDebugMessage(
				INDEX_NONE,
				30.f,
				FColor::Red,
				FString::Printf(TEXT("get from SpawnActor")));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("not valid ItemClass to UesItem"));
		}
	}
}

void UCPP_Slot::EquipSlotItem()
{
	InventoryRef->SetEquipWeapon(ItemRef, MyArrayNumber);
}

void UCPP_Slot::InactiveSlot()
{
	Super::InactiveSlot();

	/**set Text*/
	TextAmount->SetVisibility(ESlateVisibility::Hidden);
	/**set CombineButton*/
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_Slot::ActiveSlot()
{
	Super::ActiveSlot();
}

void UCPP_Slot::InitSlotInfo()
{
	InventorySlotinfo = InventoryRef->GetSlotInfoIndex(MyArrayNumber);
	ItemRef = InventorySlotinfo.Item;
	MyAmount = InventorySlotinfo.ItemAmount;
	bMyItemCanStacked = ItemRef->bCanStacked;
}


void UCPP_Slot::SearchCombinableSlot()
{
	if (ItemRef->ItemType == EItemCategory::EIS_Combinables)
	{
		CombinableSlot = InventoryRef->FindCombinableSlot(MyArrayNumber);

		if (CombinableSlot != -1)
		{
			InventoryRef->InventoryWidget->SlotWidgetArray[CombinableSlot]->ActiveCombinableSlot();
		}

		InventoryRef->ClearConectArray();
	}
}

void UCPP_Slot::InactiveCombinableSlot()
{
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
	bActiveCombineButton = false;
}

void UCPP_Slot::ActiveCombinableSlot()
{
	CombineButton->SetVisibility(ESlateVisibility::Visible);
	bActiveCombineButton = true;
}

void UCPP_Slot::CheckCombinability(const int16 fromIndex)
{
	UItem* item = InventoryRef->SlotsArray[fromIndex].Item;
	bool bvalidItem = (item == nullptr) || (item->ItemType != EItemCategory::EIS_Combinables);

	if (bvalidItem && LinkedCombinableSlot != -1)
	{
		InventoryRef->InventoryWidget->SlotWidgetArray[LinkedCombinableSlot]->InactiveCombinableSlot();

		LinkedCombinableSlot = -1;
	}
}


void UCPP_Slot::CombineItem()
{
	InventoryRef->ChangeItemInfo(ItemRef->CombinResultID, MyArrayNumber);
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
	bActiveCombineButton = false;
}

