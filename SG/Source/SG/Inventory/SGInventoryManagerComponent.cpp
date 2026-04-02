// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Inventory/SGInventoryManagerComponent.h"
#include "SGInventoryItemDefinition.h"
#include "SGInventoryItemInstance.h"

USGInventoryItemInstance* FSGInventoryList::AddEntry(TSubclassOf<USGInventoryItemDefinition> ItemDef)
{
	USGInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComp);

	// OwnerComp: USGInventoryManagerComponent
	// OwnerComp->GetOwner(): Controller
	AActor* OwningActor = OwnerComp->GetOwner();
	check(OwningActor->HasAuthority());

	FSGInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<USGInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDefinition = ItemDef;

	Result = NewEntry.Instance;
	return Result;
}

//////////////////////////////////////////////////////////////////////

USGInventoryManagerComponent::USGInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), InventoryList(nullptr)
{
}

USGInventoryItemInstance* USGInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<USGInventoryItemDefinition> ItemDef)
{
	USGInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}
	return Result;
}
