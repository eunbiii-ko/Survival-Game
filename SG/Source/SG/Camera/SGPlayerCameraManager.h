// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "SGPlayerCameraManager.generated.h"

#define SG_CAMERA_DEFAULT_FOV (80.f)
#define SG_CAMERA_DEFAULT_PITCH_MIN (-89.f)
#define SG_CAMERA_DEFAULT_PITCH_MAX (89.f)

/**
 * 
 */
UCLASS()
class SG_API ASGPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ASGPlayerCameraManager(const FObjectInitializer& ObjectInitializer);
};
