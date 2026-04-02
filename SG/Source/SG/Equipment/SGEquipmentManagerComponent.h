// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SGEquipmentManagerComponent.generated.h"

class USGEquipmentDefinition;
class USGEquipmentInstance;

//////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FSGAppliedEquipmentEntry
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
struct FSGEquipmentList
{
	GENERATED_BODY()

	FSGEquipmentList(UActorComponent* InOwnerComp = nullptr)
		: OwnerComp(InOwnerComp)
	{}

	USGEquipmentInstance* AddEntry(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(USGEquipmentInstance* Instance);
	
	/** 장착물에 대한 관리 리스트 */
	UPROPERTY()
	TArray<FSGAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComp;
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


	USGEquipmentInstance* EquipItem(TSubclassOf<USGEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(USGEquipmentInstance* ItemInstance);
	
	UPROPERTY()
	FSGEquipmentList EquipmentList;
};
