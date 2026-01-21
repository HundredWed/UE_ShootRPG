// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UE_RPG/UtilityMecro.h"
#include "Structs/ST_NPC.h"
#include "CPP_DialogueButtonBase.generated.h"

class UButton;
class UTextBlock;


UCLASS()
class UE_RPG_API UCPP_DialogueButtonBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	virtual void UpdateButtonText(const FText& buttonText);
	

protected:
	virtual void DialogueButtonEvent();

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ButtonText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* DialogueButton;

	FName ReturnRowName;
};
