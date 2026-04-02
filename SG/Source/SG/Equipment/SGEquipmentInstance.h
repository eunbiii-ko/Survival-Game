// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGEquipmentInstance.generated.h"

struct FSGEquipmentActorToSpawn;


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SG_API USGEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	USGEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * BP 정의를 위한 Equip/Unequip 함수
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
	void K2_OnUnequipped();

	UFUNCTION(BlueprintPure, Category = "Equipment")
	APawn* GetPawn() const;

	void SpawnEquipmentActors(const TArray<FSGEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	/**
	 * interfaces
	 */
	virtual void OnEquipped();
	virtual void OnUnequipped();
	
	/** 어떤 InventoryItemInstance에 의해 활성화되었는지 */
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	/** SGEquipmentDefinition에 맞게 Spawn된 Actor Isntance들 */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
