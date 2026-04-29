// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Equipment/SGGA_FromEquipment.h"

#include "SGEquipmentInstance.h"
#include "SG/Inventory/SGInventoryItemInstance.h"

USGGA_FromEquipment::USGGA_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

USGEquipmentInstance* USGGA_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		// USGGA_FromEquipment는 EquipmentInstance로부터 GiveAbility()를 진행했으므로,
		// SourceObject에 EquipmentInstance이 저장되어 있을 것이다.
		return Cast<USGEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

USGInventoryItemInstance* USGGA_FromEquipment::GetAssociatedItem() const
{
	if (USGEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<USGInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}
