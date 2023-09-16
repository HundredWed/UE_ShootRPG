// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CPP_Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CPP_Character.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Item.h"
#include "Item/ItemAbility.h"
#include "Widget/SlotDrag.h"
#include "Widget/CPP_DragSlotWidget.h"
#include "Widget/SetAmountWidget.h"
#include "Widget/TootipWidget.h"

void UCPP_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	CombineButton->OnPressed.AddDynamic(this, &UCPP_Slot::CombineItem);
	DefaultBorderColor = SlotBorder->GetBrushColor();
}

void UCPP_Slot::UpdateSlot(const uint8 index)
{
	if (IsValid(InventoryRef))
	{
		MyArrayNumber = index;
		bool isSlotEmpty = InventoryRef->IsSlotEmpty(index);

		if (isSlotEmpty)
		{
			SlotInactive();			
		}
		else
		{
			/**init this slot info*/
			InventorySlotinfo = InventoryRef->GetSlotInfoIndex(index);
			ItemRef = InventorySlotinfo.Item;
			MyAmount = InventorySlotinfo.ItemAmount;
			bMyItemCanStacked = ItemRef->bCanStacked;

			/**set icon*/
			ItemIcon->SetIsEnabled(true);
			ItemIcon->SetBrushFromTexture(ItemRef->IconTexture);
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
			/**set border*/
			SlotBorder->SetBrushColor(FLinearColor::White);

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
			if (IsValid(toolTip))
			{
				/**if created tootip before, don't create widget and update that tootip*/
				toolTip->SetTootipItemRef(ItemRef);
				toolTip->UpdateToolTip();

				ItemIcon->SetToolTip(toolTip);
			}
			else
			{
				/**CreateWidget only once*/
				if (TootipWidgetClass)
				{
					toolTip = CreateWidget<UTootipWidget>(GetWorld(), TootipWidgetClass);
					toolTip->SetTootipItemRef(ItemRef);
					toolTip->UpdateToolTip();
					//icon
					//SlotButton->SetToolTip(toolTip);
					ItemIcon->SetToolTip(toolTip);
				}
			}

			/**FindCombinableSlot*/
			if (ItemRef->ItemType == EItemCategory::EIS_Readables)
			{
				CombinableSlot = InventoryRef->FindCombinableSlot(MyArrayNumber);

				if (CombinableSlot != -1)
				{
					InventoryRef->InventoryWidget->SlotWidgetArray[CombinableSlot]->
						CombineButton->SetVisibility(ESlateVisibility::Visible);
				}
				InventoryRef->ClearConectArray();
			}
		}
	}
}


void UCPP_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UCPP_DragSlotWidget* dragWidget = CreateWidget<UCPP_DragSlotWidget>(GetWorld(), DragWidgetClass);
	dragWidget->UpdataWidget(ItemRef, MyAmount);

	USlotDrag* dragSlot = Cast<USlotDrag>(UWidgetBlueprintLibrary::CreateDragDropOperation(USlotDrag::StaticClass()));

	if (IsValid(dragSlot))
	{
		dragSlot->WidgetRef = this;
		dragSlot->DefaultDragVisual = dragWidget;
		dragSlot->Pivot = EDragPivot::MouseDown;
	}

	OutOperation = dragSlot;
}

bool UCPP_Slot::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (bDraggedOver)
	{
		return true;
	}
	else
	{
		USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
		if (dragSlot)
		{
			//UE_LOG(LogTemp, Display, TEXT("DragOver "));
			bDraggedOver = true;
			//border
			//SlotButton->SetStyle(OverStlyle);
			return true;
		}
		else
		{
			return false;
		}
	}
	
}

void UCPP_Slot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	if (bDraggedOver)
	{
		USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);
		if (dragSlot)
		{
			bDraggedOver = false;
			//border
			//SlotButton->SetStyle(DefaultStlyle);
		}
	}
}

bool UCPP_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	USlotDrag* dragSlot = Cast<USlotDrag>(InOperation);

	if (dragSlot)
	{
		const uint8 fromIndex = dragSlot->WidgetRef->MyArrayNumber;
		const uint8 toIndex = MyArrayNumber;
		if (dragSlot->WidgetRef != this)
		{
			bDraggedOver = false;

			//border
			//SlotButton->SetStyle(DefaultStlyle);

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
	
	/**확인 후 ItemIcon->GetIsEnabled() 로 통합*/

	if (ItemIcon->GetIsEnabled())
	{
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			//icon
			if (ItemIcon->GetIsEnabled())
			{
				if (IsValid(InventoryRef))
				{
					OnUseItem();
				}
			}
		}
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("SlotClickEvent "));
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
			if (IsValid(InventoryRef))
			{
				OnUseItem();
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
		//item->Destroy();
		GEngine->AddOnScreenDebugMessage(
			INDEX_NONE,
			30.f,
			FColor::Red,
			FString::Printf(TEXT("get from SpawnActor")));
	}


	
}

void UCPP_Slot::SlotInactive()
{
	/**set icon*/
	ItemIcon->SetIsEnabled(false);
	ItemIcon->SetToolTip(nullptr);
	ItemIcon->SetVisibility(ESlateVisibility::Hidden);
	/**set border*/
	SlotBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SlotBorder->SetBrushColor(DefaultBorderColor);
	/**set Text*/
	TextAmount->SetVisibility(ESlateVisibility::Hidden);
	/**set CombineButton*/
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
}

void UCPP_Slot::CombineItem()
{
	InventoryRef->CombineItem(MyArrayNumber);
	CombineButton->SetVisibility(ESlateVisibility::Hidden);
}

