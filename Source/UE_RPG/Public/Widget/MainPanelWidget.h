// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "MainPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_RPG_API UMainPanelWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UCPP_InventoryWidget* InventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class USetAmountWidget* ThrowWidget;

	FORCEINLINE UCPP_InventoryWidget* GetInventoryWidget() { return InventoryWidget; }

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override ;
};
