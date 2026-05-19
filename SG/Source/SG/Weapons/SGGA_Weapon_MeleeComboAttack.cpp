// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Weapons/SGGA_Weapon_MeleeComboAttack.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Abilities/Tasks/AbilityTask_WaitConfirmCancel.h"
#include "SG/SGGameplayTags.h"
#include "SG/SGLogChannels.h"
#include "SG/AbilitySystem/Abilities/Tasks/SGAT_WaitInputStart.h"

USGGA_Weapon_MeleeComboAttack::USGGA_Weapon_MeleeComboAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGGA_Weapon_MeleeComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}
	
	bInputPressed = false;
	bInputReleased = false;

	if (UAbilityTask_WaitInputRelease* InputReleaseTask =
		UAbilityTask_WaitInputRelease::WaitInputRelease(this, true))
	{
		InputReleaseTask->OnRelease.AddDynamic(this, &ThisClass::OnInputReleased);
		InputReleaseTask->ReadyForActivation();
	}
	
	WaitInputContinue();
	WaitInputStop();
}

void USGGA_Weapon_MeleeComboAttack::HandleMontageEvent(FGameplayEventData Payload)
{
	if (HasAuthority(&CurrentActivationInfo) == false)
	{
		return;
	}

	bool bCanContinue = NextAbilityClass && bInputPressed;

	if (Payload.EventTag == SGGameplayTags::Event_Check_Combo)
	{
		// 필요시 추가 검사
	}

	if (Payload.EventTag == SGGameplayTags::Notify_End && bCanContinue)
	{
		if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo())
		{
			bool bActivated = ASC->TryActivateAbilityByClass(NextAbilityClass);
		}
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}


void USGGA_Weapon_MeleeComboAttack::WaitInputContinue()
{
	if (USGAT_WaitInputStart* InputStartTask = USGAT_WaitInputStart::WaitInputStart(this))
	{
		InputStartTask->OnStart.AddDynamic(this, &ThisClass::OnInputStart);
		InputStartTask->ReadyForActivation();
	}
}

void USGGA_Weapon_MeleeComboAttack::WaitInputStop()
{
	if (UAbilityTask_WaitConfirmCancel* InputConfirmCancelTask = UAbilityTask_WaitConfirmCancel::WaitConfirmCancel(this))
	{
		InputConfirmCancelTask->OnCancel.AddDynamic(this, &ThisClass::OnInputCancel);
		InputConfirmCancelTask->ReadyForActivation();
	}
}

void USGGA_Weapon_MeleeComboAttack::OnInputReleased(float TimeHeld)
{
	bInputReleased = true;
}

void USGGA_Weapon_MeleeComboAttack::OnInputStart()
{
	bInputPressed = true;
	WaitInputContinue();
}

void USGGA_Weapon_MeleeComboAttack::OnInputCancel()
{
	bInputPressed = false;
	WaitInputStop();
}
