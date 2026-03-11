// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroInputComponent.h"

USGHeroInputComponent::USGHeroInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}
