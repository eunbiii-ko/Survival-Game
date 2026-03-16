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

	// 현재 CameraModeClass를 가져와서, CameraMode들을 블렌딩한다:
	UpdateCameraModes();

	FSGCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	// PC의 ConrolRotation을 EvaluateStack()에서 계산된 CameraModeView.ControlRotation로 업데이트한다:
	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	// Camera의 Location과 Rotation에 최신 값(CameraModeView)을 적용한다.
	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);

	// FOV 업데이트
	FieldOfView = CameraModeView.FieldOfView;

	// DesiredView 업데이트한다:
	// - CameraComp의 변화 사항을 반영해서 최종 렌더링까지 반영하게 된다.
	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	// See if the CameraActor wants to override the PostProcess settings used.
	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
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
