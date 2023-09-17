// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TootipWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Item/Item.h"

void UTootipWidget::UpdateToolTip()
{
	EItemCategory itemtype = ItemRef->ItemType;

	switch (itemtype)
	{
	case EItemCategory::EIS_Consumeable:
		Category->SetText(FText::FromString(TEXT("소모품")));
		ATK->SetVisibility(ESlateVisibility::Collapsed);
		BlankSpace->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemCategory::EIS_Equipment:
		Category->SetText(FText::FromString(TEXT("장비")));
		break;
	case EItemCategory::EIS_QuestItems:
		Category->SetText(FText::FromString(TEXT("퀘스트 아이템")));
		ATK->SetVisibility(ESlateVisibility::Collapsed);
		BlankSpace->SetVisibility(ESlateVisibility::Collapsed);
		Price->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemCategory::EIS_Readables:
		Category->SetText(FText::FromString(TEXT("스크롤")));
		ATK->SetVisibility(ESlateVisibility::Collapsed);
		BlankSpace->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemCategory::EIS_Gabbable:
		Category->SetText(FText::FromString(TEXT("열쇠")));
		ATK->SetVisibility(ESlateVisibility::Collapsed);
		BlankSpace->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemCategory::EIS_Combinables:
		Category->SetText(FText::FromString(TEXT("조합 재료")));
		ATK->SetVisibility(ESlateVisibility::Collapsed);
		BlankSpace->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}

	ItemIcon->SetBrushFromTexture(ItemRef->IconTexture);
	Name->SetText(ItemRef->Name);
	DescriptionText->SetText(ItemRef->Description);
	ATK->SetText(FText::Format(NSLOCTEXT("TootipWidget", "ATK", "공격력: {0}"), ItemRef->ATK));
	Price->SetText(FText::Format(NSLOCTEXT("TootipWidget", "Price", "가격: {0}"), ItemRef->ItemPrice));
	Weight->SetText(FText::Format(NSLOCTEXT("TootipWidget", "Weight", "무게: {0}"), ItemRef->Weight));
}
