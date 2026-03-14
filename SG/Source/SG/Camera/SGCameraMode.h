// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGCameraMode.generated.h"

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
	 * member variables
	 */
	/** 생성된 CameraMode들을 관리한다. */
	UPROPERTY()
	TArray<TObjectPtr<USGCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	TArray<TObjectPtr<USGCameraMode>> CameraModeStack;
};