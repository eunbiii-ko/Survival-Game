// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SGInventoryItemFragment_EquippableItem.h"
#include "SG/SGDefine.h"
#include "Templates/SubclassOf.h"
#include "SGInventoryItemFragment_Equippable_Weapon.generated.h"


class USGEquipmentDefinition;


/**
 */
UCLASS()
class USGInventoryItemFragment_Equippable_Weapon : public USGInventoryItemFragment_EquippableItem
{
	GENERATED_BODY()

public:
	USGInventoryItemFragment_Equippable_Weapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	EWeaponHandType WeaponHandType = EWeaponHandType::Count;

	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	EWeaponType WeaponType = EWeaponType::Count;
	
	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> FrontHitMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> BackHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> LeftHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> RightHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> BlockHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSoftObjectPtr<UAnimMontage> PocketWorldIdleMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "SG | Equippable Weapon")
	TSubclassOf<UAnimInstance> AnimInstanceClass;
};


