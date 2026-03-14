// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Camera/SGCameraMode.h"
#include "SGCameraMode_ThirdPerson.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SG_API USGCameraMode_ThirdPerson : public USGCameraMode
{
	GENERATED_BODY()

public:
	USGCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};


