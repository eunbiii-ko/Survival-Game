// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Camera/SGCameraMode.h"

#include "SGCameraComponent.h"
#include "SGPlayerCameraManager.h"

//////////////////////////////////////////////////////////////////////////
// FSGCameraModeView
//////////////////////////////////////////////////////////////////////////
FSGCameraModeView::FSGCameraModeView()
	: Location(ForceInit), Rotation(ForceInit), ControlRotation(ForceInit), FieldOfView(SG_CAMERA_DEFAULT_FOV)
{
}



//////////////////////////////////////////////////////////////////////////
// USGCameraMode
//////////////////////////////////////////////////////////////////////////
USGCameraMode::USGCameraMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BlendTime = 0.f;
	BlendAlpha = 1.f;
	BlendWeight = 1.f;

	FieldOfView = SG_CAMERA_DEFAULT_FOV;
	ViewPitchMin = SG_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = SG_CAMERA_DEFAULT_PITCH_MAX;

	BlendExponent = 4.f;
	BlendFunction = ESGCameraModeBlendFunction::EaseOut;
}

void USGCameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor를 활용하여 Pivot[Location|Rotation]을 계산하여, View를 업데이트한다.
	UpdateView(DeltaTime);

	UpdateBlending(DeltaTime);
}

void USGCameraMode::UpdateView(float DeltaTime)
{
	// Character의 Location과 ControlRotation을 활용하여 View를 업데이트한다:
	// CameraMode를 갖고 있는 CameraComp의 Onwer인 Character(Pawn)을 활용하여,
	// PivotLocation/Rotation을 반환한다.
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch 값에 대해 Min/Max를 Clamp 시킨다.
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FSGCameraModeView에 PivotLocation/Rotation 설정
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation을 똑같이 ControlRotation으로 활용
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void USGCameraMode::UpdateBlending(float DeltaTime)
{
	// DeltaTime, BlendTime을 이용해 BlendAlpha를 계산한다.
	if (BlendTime > 0.f)
	{
		// BlendTime은 Blending 과정 총 시간(초)
		// - BlendAlpha는 0 -> 1 로 변화하는 과정:
		// - DeltaTime을 활용하여 BlendTime을 1로 볼 경우, 진행 정도를 누적한다.
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.f;
	}

	// BlendWeight를 [0, 1]를 BlendFunction에 맞게 재매핑한다:
	const float Exponent = (BlendExponent > 0.f) ? BlendExponent : 1.f;
	switch (BlendFunction)
	{
	case ESGCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ESGCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.f, 1.f, BlendAlpha, Exponent);
		break;
	case ESGCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.f, 1.f, BlendAlpha, Exponent);
		break;
	case ESGCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.f, 1.f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

FVector USGCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight를 고려하여, ViewLocation을 반환한다.
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator USGCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// 보통 Pawn의 ControlRotation을 반환한다.
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}

USGCameraComponent* USGCameraMode::GetSGCameraComponent() const
{
	return CastChecked<USGCameraComponent>(GetOuter());
}

AActor* USGCameraMode::GetTargetActor() const
{
	const USGCameraComponent* CameraComp = GetSGCameraComponent();
	return CameraComp->GetTargetActor();
}


//////////////////////////////////////////////////////////////////////////
// USGCameraModeStack
//////////////////////////////////////////////////////////////////////////
USGCameraModeStack::USGCameraModeStack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
	int32 ExistingStackIndex = INDEX_NONE;
	// ExistingStackContribution:  CameraMode가 현재 최종 화면에서 차지하는 비율
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

void USGCameraModeStack::EvaluateStack(float DeltaTime, FSGCameraModeView& OutCameraModeView)
{
	// CameraModeStack에 있는 CameraMode를 업데이트한다:
	//  - CameraMode->BlendWeight >= 1: 삭제 
	UpdateStack(DeltaTime);
}

void USGCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack을 순회하며, CameraMode를 업데이트한다.
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	// StackIndex == 0: 가장 최신 데이터
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		USGCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		// 만약 하나라도 CameraMode가 BlendWeight가 1.0에 도달했다면,
		// 그 이후 CameraMode를 제거한다.
		if (CameraMode->BlendWeight >= 1.f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

