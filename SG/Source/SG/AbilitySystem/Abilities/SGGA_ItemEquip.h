// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SGGA_ItemEquip.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FGameplayTag EquipTag;
};
