// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_DialogueButtonBase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UCPP_DialogueButtonBase::NativeConstruct()
{
    Super::NativeConstruct();

    if (DialogueButton)
    {
        DialogueButton->OnClicked.AddDynamic(this, &UCPP_DialogueButtonBase::DialogueButtonEvent);
    }
}

void UCPP_DialogueButtonBase::UpdateButtonText(const FText& buttonText)
{
	ButtonText->SetText(buttonText);
}

void UCPP_DialogueButtonBase::DialogueButtonEvent()
{
   DISPLAYLOG(TEXT("클릭!!!!"))
}
