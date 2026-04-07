// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Player/SGPlayerController.h"

#include "SGPlayerState.h"
#include "SG/AbilitySystem/SGAbilitySystemComponent.h"
#include "SG/Camera/SGPlayerCameraManager.h"

ASGPlayerController::ASGPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ASGPlayerCameraManager::StaticClass();
}

void ASGPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	// PostProcessInput()는 한 프레임 내에서 사용자가 입력한 모든 처리가 완료되고, 호출된다.
	if (USGAbilitySystemComponent* SGASC = GetSGAbilitySystemComponent())
	{
		SGASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

ASGPlayerState* ASGPlayerController::GetSGPlayerState() const
{
	// ECastCheckedType::NullAllowed: nullptr을 반환한다.
	return CastChecked<ASGPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

USGAbilitySystemComponent* ASGPlayerController::GetSGAbilitySystemComponent() const
{
	const ASGPlayerState* SGPS = GetSGPlayerState();
	return (SGPS ? SGPS->GetSGAbilitySystemComponent() : nullptr);
}

void ASGPlayerController::ServerRequestMapTravel_Implementation(const FString& TravelURL)
{
	// 서버에서만 실행됨
	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(TravelURL);
	}
}
