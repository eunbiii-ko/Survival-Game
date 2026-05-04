// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGA_ItemEquip.generated.h"

class USGInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FSGWeaponEquip
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EquipTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USGInventoryItemDefinition> EquipmentDefinition;
};

/**
 * 
 */
UCLASS()
class SG_API USGGA_ItemEquip : public USGGameplayAbility
{
	GENERATED_BODY()
	
public:
	USGGA_ItemEquip(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	UFUNCTION(BlueprintNativeEvent)
	void EquipWeaponByTag(FGameplayEventData Data);


private:
	// Key: CosmeticTag, Value: 장착할 CharacterPart
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta= (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FSGWeaponEquip> WeaponEquipMap;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> EquippedWeaponTags;
	
};
