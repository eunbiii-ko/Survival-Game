// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGGameMode.h"

#include "SGExperienceManagerComponent.h"
#include "SGGameState.h"
#include "SG/Character/SGCharacter.h"
#include "SG/Player/SGPlayerController.h"
#include "SG/Player/SGPlayerState.h"

ASGGameMode::ASGGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ASGGameState::StaticClass();
	PlayerControllerClass = ASGPlayerController::StaticClass();
	PlayerStateClass = ASGPlayerState::StaticClass();
	DefaultPawnClass = ASGCharacter::StaticClass();
}

void ASGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로,
	// 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다.
	// -> 이를 처리하기 위해, 한 프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ASGGameMode::InitGameState()
{
	Super::InitGameState();

	USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);

	// 비동기 로딩 완료에 대한 델리게이트에 함수를 바인딩한다.
	ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(
		FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ASGGameMode::OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience)
{
}

void ASGGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
}
