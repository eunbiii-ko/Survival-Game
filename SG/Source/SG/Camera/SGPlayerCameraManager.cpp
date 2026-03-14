// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Camera/SGPlayerCameraManager.h"

ASGPlayerCameraManager::ASGPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = SG_CAMERA_DEFAULT_FOV;
	ViewPitchMin = SG_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = SG_CAMERA_DEFAULT_PITCH_MAX;
}
