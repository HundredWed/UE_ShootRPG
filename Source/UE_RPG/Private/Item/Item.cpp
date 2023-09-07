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
	copy->ATK = this->ATK;

	/**itemtype data*/
	copy->ItemType = this->ItemType;
	copy->ConsumeValue = this->ConsumeValue;
	
	/**asset data*/
	copy->ItemMesh = this->ItemMesh;
	copy->IconTexture = this->IconTexture;

	copy->bCopyItem = true;

	return copy;
}

//void UItem::UseItem(ACPP_Character* player)
//{
//	UE_LOG(LogTemp, Warning, TEXT("UseItem!!"));
//}

