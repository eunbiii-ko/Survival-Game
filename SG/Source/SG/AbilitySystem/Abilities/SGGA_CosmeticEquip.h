// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/AbilitySystem/Abilities/SGGameplayAbility.h"
#include "SG/Cosmetics/SGCharacterPartTypes.h"
#include "SGGA_CosmeticEquip.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGGA_CosmeticEquip : public USGGameplayAbility
{
	GENERATED_BODY()

public:
	USGGA_CosmeticEquip(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly)
	FSGCharacterPart NewCosmeticPart;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> CosmeticLayer;
};
