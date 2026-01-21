// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/NPC/Dialogue/CPP_DialogueButtonBase.h"
#include "CPP_DialogueCategoryButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnCategoryEventDelegate, EInteractType);

UCLASS()
class UE_RPG_API UCPP_DialogueCategoryButton : public UCPP_DialogueButtonBase
{
	GENERATED_BODY()

public:

	FOnCategoryEventDelegate OnInteractButtonEvent;

protected:
	virtual void DialogueButtonEvent() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	EInteractType InteractType = EInteractType::Normal;
	
};
