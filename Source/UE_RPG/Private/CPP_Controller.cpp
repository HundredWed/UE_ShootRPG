// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Controller.h"
#include "GameFramework/HUD.h"

void ACPP_Controller::BeginPlay()
{
	CrosshairHUD = GetHUD();

	CrosshairHUD->bShowHUD = false;
}

void ACPP_Controller::SetHUDVisibility(bool bshowHUD)
{
	if (IsValid(CrosshairHUD))
	{
		CrosshairHUD->bShowHUD = bshowHUD;
	}
}


