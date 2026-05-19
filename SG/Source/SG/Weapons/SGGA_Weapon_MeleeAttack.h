// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Weapons/SGGA_Weapon_Attack.h"
#include "SGGA_Weapon_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGGA_Weapon_MeleeAttack : public USGGA_Weapon_Attack
{
	GENERATED_BODY()

public:
	USGGA_Weapon_MeleeAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void HandleMontageEvent(FGameplayEventData Payload);
	UFUNCTION()
	void OnMontageEventTriggered(FGameplayEventData Payload);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SG | Melee Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
};
