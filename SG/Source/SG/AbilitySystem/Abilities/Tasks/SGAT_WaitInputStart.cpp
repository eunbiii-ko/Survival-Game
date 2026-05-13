// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/Abilities/Tasks/SGAT_WaitInputStart.h"

#include "AbilitySystemComponent.h"

USGAT_WaitInputStart::USGAT_WaitInputStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

USGAT_WaitInputStart* USGAT_WaitInputStart::WaitInputStart(UGameplayAbility* OwningAbility)
{
	USGAT_WaitInputStart* Task = NewAbilityTask<USGAT_WaitInputStart>(OwningAbility);
	return Task;
}

void USGAT_WaitInputStart::Activate()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC && Ability)
	{
		// GameCustom1 채널에 콜백함수를 등록한다. 
		DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GameCustom1,
			GetAbilitySpecHandle(),
			GetActivationPredictionKey()).AddUObject(this,
				&USGAT_WaitInputStart::OnStartCallback);

		// 서버에서 실행중이면 
		if (IsForRemoteClient())
		{
			// 클라이언트 이벤트가 이미 도착했으면 실행한다. 
			if (ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::GameCustom1,
				GetAbilitySpecHandle(),
				GetActivationPredictionKey()) == false)
			{
				// 아직 안왔으면 대기한다. 
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}

void USGAT_WaitInputStart::OnStartCallback()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (Ability == nullptr || ASC == nullptr)
		return;

	// 델리게이트를 정리한다.
	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::GameCustom1,
		GetAbilitySpecHandle(),
		GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());

	// 예측 가능한 클라이언트면 
	if (IsPredictingClient())
	{
		// 서버에 RPC를 전송한다.
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	// 서버라면 
	else 
	{
		// 이벤트를 처리한다.
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::GameCustom1, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnStart.Broadcast();	
	}

	// Task를 종료한다.
	EndTask();
}
