// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/Abilities/SGGA_ItemEquip.h"

USGGA_ItemEquip::USGGA_ItemEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// EquipmentInstance는 Pawn당 하나만 존재해야 하므로 InstancedPerActor 사용
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 장착 로직은 서버에서만 실행 (Authority)
	// EquipmentManagerComponent::EquipItem()에 HasAuthority() 체크가 내재되어 있음
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	ActivationPolicy = ESGAbilityActivationPolicy::OnInputTriggered;
}

void USGGA_ItemEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
