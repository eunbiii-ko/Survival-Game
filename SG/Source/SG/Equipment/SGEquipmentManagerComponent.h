// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SG/AbilitySystem/SGAbilitySet.h"
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

	/** 무기에 할당된 허용가능한 GameplayAbility */
	UPROPERTY()
	FSGAbilitySet_GrantedHandles GrantedHandles;
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
	/** 클라이언트가 새 장비 항목을 수신했을 때 */
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	/** 클라이언트에서 장비 항목이 제거되기 직전 */
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	/** 클라이언트에서 기존 항목이 변경됐을 때 */
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSGAppliedEquipmentEntry, FSGEquipmentList>(Entries, DeltaParms, *this);
	}
	
	USGEquipmentInstance* AddEntry(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(USGEquipmentInstance* Instance);

private:
	USGAbilitySystemComponent* GetAbilitySystemComponent() const;

	
public:
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

	//~UActorComponent interface
	virtual void ReadyForReplication() override;
	virtual void UninitializeComponent() override;
	//~End of UActorComponent interface
	
	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface


	
	USGEquipmentInstance* EquipItem(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(USGEquipmentInstance* ItemInstance);
	
	UPROPERTY(Replicated)
	FSGEquipmentList EquipmentList;
};
