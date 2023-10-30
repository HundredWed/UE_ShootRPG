// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CustomUMGWidget.h"
#include "CPP_Character.h"
#include "Inventory.h"

void UCustomUMGWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerRef = Cast<ACPP_Character>(GetOwningPlayerPawn());
	if (IsValid(PlayerRef))
	{
		InventoryRef = PlayerRef->GetInventory();
		if (!IsValid(InventoryRef))
			WARNINGLOG(TEXT("Found InventoryRef!!"))
	}
	else
	{
		WARNINGLOG(TEXT("Found PlayerRef!!"))
	}
}
