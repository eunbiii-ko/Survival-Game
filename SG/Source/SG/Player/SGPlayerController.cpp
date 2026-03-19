// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Player/SGPlayerController.h"
#include "SG/Camera/SGPlayerCameraManager.h"

ASGPlayerController::ASGPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ASGPlayerCameraManager::StaticClass();
}

void ASGPlayerController::ServerRequestMapTravel_Implementation(const FString& TravelURL)
{
	// 서버에서만 실행됨
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(TravelURL);
	}
}
