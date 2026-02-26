// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGGameState.h"
#include "SGExperienceManagerComponent.h"

ASGGameState::ASGGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComp = CreateDefaultSubobject<USGExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
