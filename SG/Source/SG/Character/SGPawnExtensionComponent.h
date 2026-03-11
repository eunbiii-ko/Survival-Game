// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SGPawnExtensionComponent.generated.h"

/**
 * 컴포넌트들의 초기화를 담당한다.
 */
UCLASS()
class SG_API USGPawnExtensionComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	USGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);
};


