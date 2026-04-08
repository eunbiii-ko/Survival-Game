// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SGInventoryManagerComponent.generated.h"

class USGInventoryItemInstance;
class USGInventoryItemDefinition;

//////////////////////////////////////////////////////////////////////

/** Inventory Item 단위 객체 */
USTRUCT(BlueprintType)
struct FSGInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USGInventoryItemInstance> Instance = nullptr;
};

//////////////////////////////////////////////////////////////////////

/** Inventory Item 관리 객체 */
USTRUCT(BlueprintType)
struct FSGInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FSGInventoryList(UActorComponent* InOwnerComp = nullptr)
		: OwnerComp(InOwnerComp)
	{}

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSGInventoryEntry, FSGInventoryList>(Entries, DeltaParms, *this);
	}

public:
	USGInventoryItemInstance* AddEntry(TSubclassOf<USGInventoryItemDefinition> ItemDef);
	void RemoveEntry(USGInventoryItemInstance* Instance);
	
	UPROPERTY()
	TArray<FSGInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComp;
};

template<>
struct TStructOpsTypeTraits<FSGInventoryList> : public TStructOpsTypeTraitsBase2<FSGInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};


//////////////////////////////////////////////////////////////////////

/**
 * PlayerController의 Component로서 Inventory를 관리한다.
 *  - ActorComp가 아닌 ControllerComp를 상속받아도 될 것 같은데,
 *	  일단 Lyra 기준으로 ActorComp를 상속받음. 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SG_API USGInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USGInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	/** InventoryItemDefinition을 통해 InventoryList에 추가하여 관리하며, InventoryItemInstance를 반환한다. */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	USGInventoryItemInstance* AddItemDefinition(TSubclassOf<USGInventoryItemDefinition> ItemDef);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void RemoveItemInstance(USGInventoryItemInstance* ItemInstance);

	
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface
	
	UPROPERTY(Replicated)
	FSGInventoryList InventoryList;
};


