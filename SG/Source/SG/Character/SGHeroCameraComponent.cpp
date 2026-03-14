// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroCameraComponent.h"

#include "SGPawnExtensionComponent.h"
#include "SG/SGGameplayTags.h"
#include "SG/SGLogChannels.h"
#include "SG/Camera/SGCameraComponent.h"
#include "SG/Player/SGPlayerState.h"

const FName USGHeroCameraComponent::NAME_ActorFeatureName("HeroCamera");

USGHeroCameraComponent::USGHeroCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USGHeroCameraComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	Super::OnActorInitStateChanged(Params);
}

bool USGHeroCameraComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return Super::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void USGHeroCameraComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	Super::HandleChangeInitState(Manager, CurrentState, DesiredState);

	// DataAvailable -> DataInitialized
	if (CurrentState == SGGameplayTags::InitState_DataAvailable
		&& DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ASGPlayerState* SGPS = GetPlayerState<ASGPlayerState>();
		if (!ensure(Pawn && SGPS))
		{
			return;
		}

		const USGPawnData* PawnData = nullptr;
		if (USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<USGPawnData>();
		}
		
		// CameraComp의 델리게이트(DetermineCameraModeDelegate)에 함수를 바인딩한다:
		if (PawnData)
		{
			if (USGCameraComponent* CameraComp = USGCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComp->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

TSubclassOf<USGCameraMode> USGHeroCameraComponent::DetermineCameraMode() const
{
	// PawnData 내부에 저장된 CameraModeClass를 반환한다:
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const USGPawnData* PawnData = PawnExtComp->GetPawnData<USGPawnData>())
		{
			return PawnData->CameraModeClass;
		}
	}

	return nullptr;
}
