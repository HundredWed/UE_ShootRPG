// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TootipWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UTootipWidget::InitToolTip(const FItemInfoTable* itemInfo)
{
	ATKText->SetVisibility(ESlateVisibility::Collapsed);
	BlankSpace->SetVisibility(ESlateVisibility::Collapsed);

	SetItemType(itemInfo);
	
	Price->SetText(FText::Format(NSLOCTEXT("TootipWidget", "Price", "가격: {0}"), itemInfo->ItemPrice));
	Weight->SetText(FText::Format(NSLOCTEXT("TootipWidget", "Weight", "무게: {0}"), itemInfo->Weight));
}

void UTootipWidget::InitToolTip(const FItemInfoTable* itemInfo, const FEquipmentInfoTable* equipmentInfo)
{
	SetItemType(itemInfo);

	ATKText->SetText(FText::Format(NSLOCTEXT("TootipWidget", "ATK", "공격력: {0}"), equipmentInfo->ATK));
	Weight->SetText(FText::Format(NSLOCTEXT("TootipWidget", "Weight", "무게: {0}"), itemInfo->Weight));
}

void UTootipWidget::SetItemType(const FItemInfoTable* itemInfo)
{
	ItemIcon->SetBrushFromTexture(itemInfo->IconTexture);
	Name->SetText(itemInfo->Name);
	DescriptionText->SetText(itemInfo->Description);

	switch (itemInfo->ItemType)
	{
	case EItemCategory::EIC_Consumable:
		Category->SetText(FText::FromString(TEXT("소모품")));
		break;
	case EItemCategory::EIC_Equipment:
		Category->SetText(FText::FromString(TEXT("장비")));
		break;
	case EItemCategory::EIC_QuestItems:
		Category->SetText(FText::FromString(TEXT("퀘스트 아이템")));
		Price->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemCategory::EIC_ReadItem:
		Category->SetText(FText::FromString(TEXT("스크롤")));
		break;
	case EItemCategory::EIC_Gabbable:
		Category->SetText(FText::FromString(TEXT("열쇠")));
		break;
	case EItemCategory::EIC_Combinables:
		Category->SetText(FText::FromString(TEXT("조합 재료")));
		break;
	default:
		break;
	}
}
