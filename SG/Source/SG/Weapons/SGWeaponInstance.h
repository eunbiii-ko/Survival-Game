// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Cosmetics/SGCosmeticAnimationTypes.h"
#include "SG/Equipment/SGEquipmentInstance.h"
#include "SGWeaponInstance.generated.h"


/**
 * 
 */
UCLASS()
class SG_API USGWeaponInstance : public USGEquipmentInstance
{
	GENERATED_BODY()
public:
	USGWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Weapon에 적용할 AnimLayer를 선택하여 반환한다. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Animation")
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;
	
	/** Weapon의 Equip/UnEquip에 대한 Animation Set 정보를 저장한다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FSGAnimLayerSelectionSet EquippedAnimSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FSGAnimLayerSelectionSet UnEquippedAnimSet;
};
