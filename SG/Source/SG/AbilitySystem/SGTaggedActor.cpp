// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/SGTaggedActor.h"

ASGTaggedActor::ASGTaggedActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASGTaggedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGTaggedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(StaticGameplayTags);
}

