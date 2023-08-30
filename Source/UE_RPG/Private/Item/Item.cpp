#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"



UItem::UItem()
{
	bCopyItem = false;
}

UItem* UItem::CreateItemCopy()
{
	/**the case other get other inventory. not PickUp */
	UItem* copy = NewObject<UItem>(StaticClass());

	copy->ItemInfoID = this->ItemInfoID;
	copy->Name = this->Name;
	copy->ItemMesh = this->ItemMesh;
	copy->Description = this->Description;
	copy->IconTexture = this->IconTexture;
	copy->bCanBeUsed = this->bCanBeUsed;
	copy->bCanStacked = this->bCanStacked;
	copy->UseText = this->UseText;
	copy->Interaction = this->Interaction;
	copy->ItemPrice = this->ItemPrice;
	copy->ItemType = this->ItemType;
	copy->ATK = this->ATK;

	copy->bCopyItem = true;

	return copy;
}
