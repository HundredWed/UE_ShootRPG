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

	/**item data*/
	copy->ItemInfoID = this->ItemInfoID;
	copy->Name = this->Name;
	copy->Description = this->Description;
	copy->bCanBeUsed = this->bCanBeUsed;
	copy->bCanStacked = this->bCanStacked;
	copy->UseText = this->UseText;
	copy->Interaction = this->Interaction;
	copy->ItemPrice = this->ItemPrice;
	copy->Weight = this->Weight;
	copy->ATK = this->ATK;

	/**itemtype data*/
	copy->ItemType = this->ItemType;
	copy->ConsumeValue = this->ConsumeValue;
	copy->CombinResultID = this->CombinResultID;
	if (this->ItemClass)
	{
		copy->ItemClass = this->ItemClass;
	}
	
	/**asset data*/
	copy->ItemMesh = this->ItemMesh;
	copy->IconTexture = this->IconTexture;
	copy->ItemSkeletalMesh = this->ItemSkeletalMesh;
	copy->FireParticle = this->FireParticle;

	copy->bCopyItem = true;

	return copy;
}


