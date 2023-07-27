// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GrabbableItem.h"

AGrabbableItem::AGrabbableItem()
{
	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Root"));
	RootComponent = RootMesh;
}
