// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Player/SGPlayerState.h"
#include "SG/GameModes/SGExperienceManagerComponent.h"


void ASGPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);

	ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(
		FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ASGPlayerState::OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience)
{
}
