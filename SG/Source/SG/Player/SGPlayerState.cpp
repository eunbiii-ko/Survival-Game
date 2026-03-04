// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Player/SGPlayerState.h"
#include "SG/GameModes/SGExperienceManagerComponent.h"
#include "SG/GameModes/SGGameMode.h"


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
	if (ASGGameMode* GameMode = GetWorld()->GetAuthGameMode<ASGGameMode>())
	{
		const USGPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ASGPlayerState::SetPawnData(const USGPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 2번 중복으로 설정되는 것을 막기 위함
	check(!PawnData);

	PawnData = InPawnData;
}
