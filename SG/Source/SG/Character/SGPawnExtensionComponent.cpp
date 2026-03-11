// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGPawnExtensionComponent.h"

USGPawnExtensionComponent::USGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}