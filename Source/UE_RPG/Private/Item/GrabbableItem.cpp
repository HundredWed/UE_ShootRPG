// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/GrabbableItem.h"
#include "Components/SphereComponent.h"

AGrabbableItem::AGrabbableItem()
{
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetAngularDamping(1.f);
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
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetEnableGravity(true);
		//StaticMesh->SetVisibility(true);
		StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		break;
	case EItemState::EIS_Equipped:
		/**item mesh*/
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetEnableGravity(true);
		//StaticMesh->SetVisibility(true);
		StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		/**overlap sphere*/
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}
