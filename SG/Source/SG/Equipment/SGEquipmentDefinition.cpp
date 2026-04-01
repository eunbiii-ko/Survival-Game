// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Equipment/SGEquipmentDefinition.h"
#include "SGEquipmentInstance.h"

USGEquipmentDefinition::USGEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = USGEquipmentInstance::StaticClass();
}
