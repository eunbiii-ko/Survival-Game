// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SGHeroInputComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class SG_API USGHeroInputComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	USGHeroInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
