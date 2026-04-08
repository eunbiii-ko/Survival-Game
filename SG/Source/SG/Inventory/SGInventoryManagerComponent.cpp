// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Inventory/SGInventoryManagerComponent.h"
#include "SGInventoryItemDefinition.h"
#include "SGInventoryItemInstance.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

void FSGInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FSGInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

void FSGInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
}

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

	MarkItemDirty(NewEntry);
	
	return Result;
}

void FSGInventoryList::RemoveEntry(USGInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FSGInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////

USGInventoryManagerComponent::USGInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void USGInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

USGInventoryItemInstance* USGInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<USGInventoryItemDefinition> ItemDef)
{
	USGInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void USGInventoryManagerComponent::RemoveItemInstance(USGInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

bool USGInventoryManagerComponent::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                                       FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FSGInventoryEntry& Entry : InventoryList.Entries)
	{
		USGInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void USGInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ULyraInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FSGInventoryEntry& Entry : InventoryList.Entries)
		{
			USGInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}