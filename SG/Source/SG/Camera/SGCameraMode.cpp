// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Camera/SGCameraMode.h"

//////////////////////////////////////////////////////////////////////////
// USGCameraMode
//////////////////////////////////////////////////////////////////////////
USGCameraMode::USGCameraMode(const FObjectInitializer& ObjectInitializer)
{
}


//////////////////////////////////////////////////////////////////////////
// USGCameraModeStack
//////////////////////////////////////////////////////////////////////////
USGCameraModeStack::USGCameraModeStack(const FObjectInitializer& ObjectInitializer)
{
}

void USGCameraModeStack::PushCameraMode(const TSubclassOf<USGCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	USGCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] 가장 최근에 이미 CameraMode가 Stacking 되었으므로 return
		return;
	}

	// CameraModeStack을 순회하며 CameraMode의 Weight를 계산한다:
	// ExistingStackIndex: CameraModeStack에서 CameraMode에 맞는 Index를 찾는다.
	// ExistingStackContribution: 위에서 아래로 최종 BlendWeight 값을 찾기 위해 초기값으로 1.f 설정한다.
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.f;

	for (int32 StackIndex = 0; StackIndex < StackSize; StackIndex++)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			// BlendWeight를 CameraMode에 추가한다.
			// 여기서 ExistingStackContribution 우리가 찾는 CameraMode를 찾았으니까,
			// 누적된 BlendWeight와 함께 BlendWeight를 곱하여, 루프를 빠져나온다.
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// 원하는 CameraMode가 아니므로,
			// InvBlendWeight = (1.0 - BlendWeight)를 곱해줘야 값이 누적된다.
			ExistingStackContribution *= (1.f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}

	// 우리는 CameraMode를 Top으로 반영하기 위해, 중간에 있었다면 제거해서 다시 Push 해줘야 한다.
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// 없었으니 Contribution은 0으로 설정
		ExistingStackContribution = 0.f;
	}

	// BlendTime이 0보다 크다는 것은 Blend를 얼마 시간동안 진행함을 의미하므로 ExistingStackContribution을 적용한다.
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.f);
	CameraMode->BlendWeight = BlendWeight;

	// Array를 Stack처럼 사용하므로 0번째에 추가한다.
	CameraModeStack.Insert(CameraMode, 0);

	// 마지막 BlendWeight는 항상 1.f가 되어야 한다.
	// 시간의 마지막 기준은 1.0이기 때문.
	CameraModeStack.Last()->BlendWeight = 1.f;
}

USGCameraMode* USGCameraModeStack::GetCameraModeInstance(const TSubclassOf<USGCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	// CameraModeInstances에서 먼저 생성되었는지(존재하는지) 확인 후, 반환한다.
	// CameraModeInstances: CameraModeClass를 캐싱해두는 용도
	for (USGCameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass에 맞는 CameraMode 인스턴스가 없다면 생성한다.
	USGCameraMode* NewCameraMode = NewObject<USGCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);

	return NewCameraMode;
}

