// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CharacterSpringArm.h"

UCharacterSpringArm::UCharacterSpringArm()
{
	PrimaryComponentTick.bCanEverTick = true;
	NewValue = 0;
}

void UCharacterSpringArm::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateSpringArmOffset(NewValue, DeltaTime);
}

void UCharacterSpringArm::UpdateSpringArmOffset(float NewYoffset, float DeltaTime)
{
	SocketOffset.Y = FMath::FInterpTo(SocketOffset.Y, NewYoffset, DeltaTime, 15.0f);
}
