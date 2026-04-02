// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "SGInventoryItemFragment_EquippableItem.generated.h"


class USGEquipmentDefinition;


/**
 */
UCLASS()
class USGInventoryItemFragment_EquippableItem : public USGInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SG")
	TSubclassOf<USGEquipmentDefinition> EquipmentDefinition;
};
