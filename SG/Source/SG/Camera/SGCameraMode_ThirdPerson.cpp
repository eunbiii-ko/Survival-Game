// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Camera/SGCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"
#include "SG/SGLogChannels.h"

USGCameraMode_ThirdPerson::USGCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// TargetOffsetCurve가 오버라이드되어 있다면, Curve에 값을 가져와서 적용한다.
	// Camera 관점에서 Character의 어느 부분에 Target으로 할지 결정하는 것으로 이해하면 된다.
	if (TargetOffsetCurve)
	{
		// Pitch값에 맞춰서 벡터를 가져온다.
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
