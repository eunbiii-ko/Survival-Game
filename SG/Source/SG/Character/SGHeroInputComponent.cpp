// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroInputComponent.h"

const FName USGHeroInputComponent::NAME_ActorFeatureName("HeroInput");

USGHeroInputComponent::USGHeroInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USGHeroInputComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	Super::OnActorInitStateChanged(Params);
}

bool USGHeroInputComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return Super::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void USGHeroInputComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	Super::HandleChangeInitState(Manager, CurrentState, DesiredState);
}
