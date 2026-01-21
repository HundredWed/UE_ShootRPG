// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NPC/Dialogue/CPP_AnswerBox.h"
#include "Components/VerticalBox.h"

void UCPP_AnswerBox::AddAnswerList(UUserWidget* answser)
{
	AnswerListBox->AddChild(answser);
}
