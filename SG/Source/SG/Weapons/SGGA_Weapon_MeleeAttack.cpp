// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Weapons/SGGA_Weapon_MeleeAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

USGGA_Weapon_MeleeAttack::USGGA_Weapon_MeleeAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGGA_Weapon_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// GameplayEvent_Trace 이벤트가 발생하면 콜백함수로 OnTargetDataReady() 함수를 호출한다.
	// if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, D1GameplayTags::GameplayEvent_Trace, nullptr, false, true))
	// {
	// 	GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnTargetDataReady);
	// 	GameplayEventTask->ReadyForActivation();
	// }

	if (UAbilityTask_PlayMontageAndWait* PlayMontageTask =
		UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("MeleeAttack"),
			AttackMontage,
			1.f,
			NAME_None,
			false,
			1.f,
			0.f,
			false))
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayMontageTask->ReadyForActivation();
	}
}
