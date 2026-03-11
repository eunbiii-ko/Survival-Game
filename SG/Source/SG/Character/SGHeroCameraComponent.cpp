// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroCameraComponent.h"

const FName USGHeroCameraComponent::NAME_ActorFeatureName("HeroCamera");

USGHeroCameraComponent::USGHeroCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USGHeroCameraComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	Super::OnActorInitStateChanged(Params);
}

bool USGHeroCameraComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return Super::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void USGHeroCameraComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	Super::HandleChangeInitState(Manager, CurrentState, DesiredState);
}
