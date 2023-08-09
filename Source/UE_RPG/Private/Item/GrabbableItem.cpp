// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GrabbableItem.h"

AGrabbableItem::AGrabbableItem()
{
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetAngularDamping(1.f);
}
