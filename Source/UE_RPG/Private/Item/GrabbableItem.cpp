// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GrabbableItem.h"
#include "Components/SphereComponent.h"

AGrabbableItem::AGrabbableItem()
{
	PickUpMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	PickUpMesh->SetAngularDamping(1.f);
}

void AGrabbableItem::BeginPlay()
{
	Super::BeginPlay();

	SetItemState(EItemState::EIS_UnEquipped);
}

void AGrabbableItem::SetItemState(EItemState State)
{
	/**ECC_Visibility = Item Search Trace*/
	/**ECC_GameTraceChannel1 = Grab Trace*/
	/**ECC_GameTraceChannel2 = Gun(hit) Trace*/
	switch (State)
	{
	case EItemState::EIS_UnEquipped:
		/**item mesh*/
		PickUpMesh->SetSimulatePhysics(true);
		PickUpMesh->SetEnableGravity(true);
		//StaticMesh->SetVisibility(true);
		PickUpMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		PickUpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	case EItemState::EIS_Equipped:
		/**item mesh*/
		PickUpMesh->SetSimulatePhysics(true);
		PickUpMesh->SetEnableGravity(true);
		//StaticMesh->SetVisibility(true);
		PickUpMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PickUpMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		PickUpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}
