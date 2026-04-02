// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Equipment/SGEquipmentManagerComponent.h"
#include "SGEquipmentDefinition.h"
#include "SGEquipmentInstance.h"

USGEquipmentInstance* FSGEquipmentList::AddEntry(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition)
{
	USGEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComp);
	check(OwnerComp->GetOwner()->HasAuthority());

	// EquipmentDefinition의 멤버 변수들은 EditDefaultsOnly로 정의되어 있어
	// GetDefault로 들고 와도 우리에게 필요한 것들이 모두 포함되어 있다.
	const USGEquipmentDefinition* EquipmentCDO = GetDefault<USGEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<USGEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = USGEquipmentInstance::StaticClass();
	}

	// Entries에 추가한다.
	FSGAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<USGEquipmentInstance>(OwnerComp->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	// ActorsToSpawn을 통해, Actor들을 인스턴스화한다.
	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FSGEquipmentList::RemoveEntry(USGEquipmentInstance* Instance)
{
	// Entries를 순회하며, Instance를 찾는다.
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FSGAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// Actor 제거 및 iterator를 통해 안전하게 Array에서 삭제
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

//////////////////////////////////////////////////////////////////////

USGEquipmentManagerComponent::USGEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), EquipmentList(this)
{
}

USGEquipmentInstance* USGEquipmentManagerComponent::EquipItem(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition)
{
	USGEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		// EquipmentList에서 추가한다.
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			// BP의 Evnet 노드를 호출한다.
			Result->OnEquipped();
		}
	}

	return Result;
}

void USGEquipmentManagerComponent::UnequipItem(USGEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		// BP의 Evnet 노드를 호출한다.
		ItemInstance->OnUnequipped();

		// EquipmentList에서 제거한다.
		EquipmentList.RemoveEntry(ItemInstance);
	}
}
