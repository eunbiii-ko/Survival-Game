// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGA_FromEquipment.generated.h"

class USGEquipmentInstance;
class USGInventoryItemInstance;


/**
 * 
 */
UCLASS()
class SG_API USGGA_FromEquipment : public USGGameplayAbility
{
	GENERATED_BODY()

public:
	USGGA_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "SG | Ability")
	USGEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "SG | Ability")
	USGInventoryItemInstance* GetAssociatedItem() const;
};
