// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SGEquipmentManagerComponent.generated.h"

class USGEquipmentDefinition;
class USGEquipmentInstance;

//////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FSGAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	/** 장착물에 대한 메타 데이터 */
	UPROPERTY()
	TSubclassOf<USGEquipmentDefinition> EquipmentDefinition;

	/** EquipmentDefinition을 통해 생성된 인스턴스 */
	UPROPERTY()
	TObjectPtr<USGEquipmentInstance> Instance = nullptr;
};

//////////////////////////////////////////////////////////////////////

/**
 * EquipmentInstance의 인스턴스를 Entry에서 관리한다.
 * - FSGEquipmentList는 생성된 객체를 관리한다.
 */
USTRUCT(BlueprintType)
struct FSGEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FSGEquipmentList(UActorComponent* InOwnerComp = nullptr)
		: OwnerComp(InOwnerComp)
	{}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSGAppliedEquipmentEntry, FSGEquipmentList>(Entries, DeltaParms, *this);
	}
	
	USGEquipmentInstance* AddEntry(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(USGEquipmentInstance* Instance);
	
	/** 장착물에 대한 관리 리스트 */
	UPROPERTY()
	TArray<FSGAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComp;
};

template<>
struct TStructOpsTypeTraits<FSGEquipmentList> : public TStructOpsTypeTraitsBase2<FSGEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

//////////////////////////////////////////////////////////////////////

/**
 * Pawn의 Component로서 장착물에 대한 관리를 담당한다. 
 */
UCLASS()
class SG_API USGEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	USGEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	virtual void ReadyForReplication() override;
	
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface


	
	USGEquipmentInstance* EquipItem(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(USGEquipmentInstance* ItemInstance);
	
	UPROPERTY(Replicated)
	FSGEquipmentList EquipmentList;
};
