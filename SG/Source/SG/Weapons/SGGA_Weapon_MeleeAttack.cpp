// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Weapons/SGGA_Weapon_MeleeAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "SG/SGGameplayTags.h"

USGGA_Weapon_MeleeAttack::USGGA_Weapon_MeleeAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGGA_Weapon_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo)) return;

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
		PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		PlayMontageTask->ReadyForActivation();
	}

	if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, SGGameplayTags::Event_Check_Combo,
		nullptr,
		false, true))
	{
		GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnMontageEventTriggered);
		GameplayEventTask->ReadyForActivation();
	}

	if (UAbilityTask_WaitGameplayEvent* GameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, SGGameplayTags::Notify_End,
		nullptr,
		true, true))
	{
		GameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::OnMontageEventTriggered);
		GameplayEventTask->ReadyForActivation();
	}
}

void USGGA_Weapon_MeleeAttack::HandleMontageEvent(FGameplayEventData Payload)
{
	// if (HasAuthority(&CurrentActivationInfo))
	// {
	// 	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	// }
}

void USGGA_Weapon_MeleeAttack::OnMontageEventTriggered(FGameplayEventData Payload)
{
	HandleMontageEvent(Payload);
}
