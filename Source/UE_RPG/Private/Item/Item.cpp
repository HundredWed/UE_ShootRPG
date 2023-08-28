#include "Item/Item.h"
#include "CPP_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"



UItem::UItem()
{
}

UItem* UItem::CreateItemCopy()
{
	UItem* copy = NewObject<UItem>(StaticClass());

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

	return copy;
}
