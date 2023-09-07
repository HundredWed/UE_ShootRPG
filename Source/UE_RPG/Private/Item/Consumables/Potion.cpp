// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Consumables/Potion.h"
#include "CPP_Character.h"


void APotion::UseItem(ACPP_Character* player, const uint32 consumeValue)
{
	UE_LOG(LogTemp, Display, TEXT("Use HpPortion!!"));
	player->IncreasePlayerHP(consumeValue);
}
