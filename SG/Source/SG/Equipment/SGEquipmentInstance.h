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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category = "Equipment")
	UObject* GetInstigator() const { return Instigator; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
	
	/**
	 * BP 정의를 위한 Equip/Unequip 함수
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();
	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnUnequipped"))
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

	/**
	 * DeterminesOutputType은 C++ 정의에는 APawn* 을 반환하지만,
	 * BP에서는 PawnType에 따라 OutputType이 결정되도록
	 * Redirect(PawnType에 맞게 변환)한다.
	 */
	UFUNCTION(BlueprintPure, Category = "Equipment",
		meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

private:
	UFUNCTION()
	void OnRep_Instigator();

public:
	/** 어떤 InventoryItemInstance에 의해 활성화되었는지 */
	UPROPERTY(ReplicatedUsing=OnRep_Instigator)
	TObjectPtr<UObject> Instigator;

	/** SGEquipmentDefinition에 맞게 Spawn된 Actor Isntance들 */
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
