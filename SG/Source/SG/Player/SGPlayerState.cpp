// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Player/SGPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "SG/GameModes/SGExperienceManagerComponent.h"
#include "SG/GameModes/SGGameMode.h"


void ASGPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
		check(ExperienceManagerComp);

		ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(
			FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ASGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
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

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	// PawnData가 2번 중복으로 설정되는 것을 막기 위함
	check(!PawnData);

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;
	
	ForceNetUpdate();
}

void ASGPlayerState::OnRep_PawnData()
{
}
