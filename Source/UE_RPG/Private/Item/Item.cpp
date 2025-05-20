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
	copy->ItemInfoTable = this->ItemInfoTable;

	return copy;
}


