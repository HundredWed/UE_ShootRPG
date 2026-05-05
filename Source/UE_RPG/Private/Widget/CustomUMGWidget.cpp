// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CustomUMGWidget.h"
#include "Systems/CPP_UIEventHubSubsystem.h"

void UCustomUMGWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCustomUMGWidget::CloseWidget()
{
	UCPP_UIEventHubSubsystem* Hub = GetGameInstance()->GetSubsystem<UCPP_UIEventHubSubsystem>();
	if (Hub)
	{
		Hub->OnRequestHideCursor.Execute();
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCustomUMGWidget::OpenWidget()
{
	UCPP_UIEventHubSubsystem* Hub = GetGameInstance()->GetSubsystem<UCPP_UIEventHubSubsystem>();
	if (Hub)
	{
		Hub->OnRequestShowCursor.Execute();
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UCustomUMGWidget::SetWidgetVisibility()
{
	ESlateVisibility visible = GetVisibility();
	switch (visible)
	{
	case ESlateVisibility::Visible:
		CloseWidget();
		break;
	case ESlateVisibility::Hidden:
		OpenWidget();
		break;
	}
}
