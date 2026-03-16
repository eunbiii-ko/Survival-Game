// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Camera/SGCameraComponent.h"

#include "SGCameraMode.h"

USGCameraComponent::USGCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), CameraModeStack(nullptr)
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

	// 현재 CameraModeClass를 가져와서, CameraMode들을 블렌딩한다.
	UpdateCameraModes();

	FSGCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);
}

void USGCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	
	//if (CameraModeStack->IsStackActivate())
	{
		// DetermineCameraModeDelegate에 함수가 바인딩되어 있다면
		if (DetermineCameraModeDelegate.IsBound())
		{
			// DetermineCameraModeDelegate: CameraMode Class를 반환하는 델리게이트
			if (const TSubclassOf<USGCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
			{
				// 현재 플레이어가 사용하고 있는 CameraMode를 CameraModeStack에 저장한다. 
				CameraModeStack->PushCameraMode(CameraMode);
			}
		}
	}
}
