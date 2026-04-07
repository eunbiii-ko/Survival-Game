// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
