// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGCameraMode.generated.h"

class USGCameraComponent;

/**
 * [0, 1]을 BlendFunction에 맞게 재매핑을 위한 타입
 */
UENUM()
enum class ESGCameraModeBlendFunction : uint8
{
	Linear,
	/**
	 * Ease In/Out은 exponent 값에 의해 조절된다.
	 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};


/**
 * FSGCameraModeView
 *
 *	현재 카메라가 바라보는 시야를 정의한다.
 */
struct FSGCameraModeView
{
	FSGCameraModeView();

	void Blend(const FSGCameraModeView& Other, float OtherWeight);

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

	/**
	* member methods
	*/
	void UpdateCameraMode(float DeltaTime);

protected:
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;
	USGCameraComponent* GetSGCameraComponent() const;
	AActor* GetTargetActor() const;
	
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

	/** CameraMode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View",
		meta = (UIMin = "5.0", UIMax = "170.0", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	/** View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View",
		meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;
	UPROPERTY(EditDefaultsOnly, Category = "View",
		meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;

	/**
	* EaseIn/Out에 사용한 Exponent
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent;

	/** Blend function */
	ESGCameraModeBlendFunction BlendFunction;
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
	void BlendStack(FSGCameraModeView& OutCameraModeView) const;
	
	/**
	 * member variables
	 */
	/** 생성된 CameraMode들을 관리한다. */
	UPROPERTY()
	TArray<TObjectPtr<USGCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	TArray<TObjectPtr<USGCameraMode>> CameraModeStack;
};