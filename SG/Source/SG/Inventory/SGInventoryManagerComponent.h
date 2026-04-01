// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SGInventoryManagerComponent.generated.h"

class USGInventoryItemInstance;

//////////////////////////////////////////////////////////////////////

/** Inventory Item 단위 객체 */
USTRUCT(BlueprintType)
struct FSGInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<USGInventoryItemInstance> Instance = nullptr;
};

//////////////////////////////////////////////////////////////////////

/** Inventory Item 관리 객체 */
USTRUCT(BlueprintType)
struct FSGInventoryList
{
	GENERATED_BODY()

	FSGInventoryList(UActorComponent* InOwnerComp = nullptr)
		: OwnerComp(InOwnerComp)
	{}

	UPROPERTY()
	TArray<FSGInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComp;
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
	// Sets default values for this component's properties
	USGInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	FSGInventoryList InventoryList;
};
