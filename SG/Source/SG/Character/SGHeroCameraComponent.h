// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "SGHeroCameraComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class SG_API USGHeroCameraComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	USGHeroCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());	
};
