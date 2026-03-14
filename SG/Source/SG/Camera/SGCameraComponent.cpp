// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Camera/SGCameraComponent.h"

#include "SGCameraMode.h"

USGCameraComponent::USGCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGCameraComponent::OnRegister()
{
	Super::OnRegister();

	// CameraComp가 캐릭터에 부착될 때, CameraModeStack을 만든다.
	if (!CameraModeStack)
	{
		// NewObject<>(): 초기화(BeginPlay())가 필요없는 객체로 만든다.
		CameraModeStack = NewObject<USGCameraModeStack>(this);
	}
}

void USGCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	Super::GetCameraView(DeltaTime, DesiredView);
}

void USGCameraComponent::UpdateCameraMode()
{
}
