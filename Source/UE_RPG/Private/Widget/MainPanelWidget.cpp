// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MainPanelWidget.h"
#include "CPP_Character.h"
#include "Widget/CPP_InventoryWidget.h"
#include "Inventory.h"

void UMainPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(PlayerRef))
	{
		if (IsValid(InventoryWidget))
		{
			InventoryWidget->GenerateSlotWidget(PlayerRef->GetInventoryRowSize());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("not Found InventoryWidget at MainPanel!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not Found PlayerRef at MainPanel!!"));
	}
}
