// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGGameMode.h"
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
