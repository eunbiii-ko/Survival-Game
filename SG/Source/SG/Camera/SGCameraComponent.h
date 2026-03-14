// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SGCameraComponent.generated.h"

class USGCameraMode;
class USGCameraModeStack;

/** template forward declaration */
template <class TClass> class TSubclassOf;

/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<USGCameraMode>, FSGCameraModeDelegate);

/**
 * 
 */
UCLASS()
class SG_API USGCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	USGCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	

	/** 현재 CameraMode를 가져온다. */
	FSGCameraModeDelegate DetermineCameraModeDelegate;

	
protected:
	/**
	 * CameraComponent interface
	 */
	virtual void OnRegister() override final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override final;


	/**
	 * member methods
	 */
	void UpdateCameraMode();
	
	
	/**
	 * member variables
	 */
	/** Camera Blending을 지원하는 Stack */
	UPROPERTY()
	TObjectPtr<USGCameraModeStack> CameraModeStack;

	
};
