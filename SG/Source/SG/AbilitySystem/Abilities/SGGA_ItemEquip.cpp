// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/Abilities/SGGA_ItemEquip.h"

USGGA_ItemEquip::USGGA_ItemEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGGA_ItemEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
