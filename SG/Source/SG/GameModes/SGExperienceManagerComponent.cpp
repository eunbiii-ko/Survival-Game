// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGExperienceManagerComponent.h"

#include "Net/UnrealNetwork.h"


USGExperienceManagerComponent::USGExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}


void USGExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnSGExperienceLoaded::FDelegate&& Delegate)
{
	// 이미 Experience가 로드된 경우, 바로 델리게이트를 호출한다.
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	// 만약 Experience가 아직 로드되지 않은 경우, 델리게이트를 등록(추가)한다.
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void USGExperienceManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentExperience);
}

void USGExperienceManagerComponent::OnRep_CurrentExperience()
{
}
