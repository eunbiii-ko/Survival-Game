// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Weapons/SGWeaponInstance.h"

USGWeaponInstance::USGWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSubclassOf<UAnimInstance> USGWeaponInstance::PickBestAnimLayer(bool bEquipped,
	const FGameplayTagContainer& CosmeticTags) const
{
	const FSGAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnEquippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
