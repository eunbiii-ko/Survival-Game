// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/Abilities/SGGA_ItemEquip.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

USGGA_ItemEquip::USGGA_ItemEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// EquipmentInstance는 Pawn당 하나만 존재해야 하므로 InstancedPerActor 사용
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// 몽타주 재생은 클라이언트에서도 실행돼야 함. (단, 장착 로직은 서버에서만 실행 (Authority)) 
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ActivationPolicy = ESGAbilityActivationPolicy::OnInputTriggered;
}

void USGGA_ItemEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		AActor* AvatarActor = ActorInfo->AvatarActor.Get();
		
		UAbilityTask_PlayMontageAndWait* Montage =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this, NAME_None, EquipMontage, 1.f);
		Montage->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);
		Montage->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		Montage->ReadyForActivation();



		UAbilityTask_WaitGameplayEvent* EquipWeaponEvent =
			UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, EquipTag,
				nullptr, true, true);
		EquipWeaponEvent->EventReceived.AddDynamic(this, &ThisClass::EquipWeaponByTag);
		EquipWeaponEvent->ReadyForActivation();	
	}
}

void USGGA_ItemEquip::EquipWeaponByTag_Implementation(FGameplayEventData Data)
{
	EquipWeaponByTag(Data);
}


