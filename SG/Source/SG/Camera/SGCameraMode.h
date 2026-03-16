// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGCameraMode.generated.h"

/**
 * FSGCameraModeView
 *
 *	현재 카메라가 바라보는 시야를 정의한다.
 */
struct FSGCameraModeView
{
	FSGCameraModeView();

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};


/**
 * SGCameraMode
 *  - Camera Blending 대상 유닛
 */
UCLASS(Abstract, NotBlueprintable)
class SG_API USGCameraMode : public UObject
{
	GENERATED_BODY()

public:
	USGCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
public:
	/** 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	/** 선형적인 Blend 값 [0, 1] */
	float BlendAlpha;

	/**
	 * 해당 CameraMode의 최종 Blend값 (시간에 따른 비율)
	 * 앞서 BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 계산한다.
	 */
	float BlendWeight;

	/** CameraMode에 의해 생성된 CameraModeView */
	FSGCameraModeView View;
};


/**
 * USGCameraModeStack
 *  - Camera Blending을 담당한다.
 */
UCLASS()
class USGCameraModeStack : public UObject
{
	GENERATED_BODY()

public:
	USGCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * member methods
	 */
	void PushCameraMode(const TSubclassOf<USGCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FSGCameraModeView& OutCameraModeView);
	
protected:
	/**
	 * member methods
	 */
	USGCameraMode* GetCameraModeInstance(const TSubclassOf<USGCameraMode>& CameraModeClass);
	void UpdateStack(float DeltaTime);
	
	/**
	 * member variables
	 */
	/** 생성된 CameraMode들을 관리한다. */
	UPROPERTY()
	TArray<TObjectPtr<USGCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	TArray<TObjectPtr<USGCameraMode>> CameraModeStack;
};