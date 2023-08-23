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
		UE_LOG(LogTemp, Display, TEXT("Found Player!!"));

		InventoryRef = PlayerRef->GetInventory();
		if (IsValid(InventoryRef))
		{
			UE_LOG(LogTemp, Display, TEXT("Found InventoryRef!!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Found InventoryRef!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("not Found Player!!"));
	}
}
