// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGExperienceDefinition.generated.h"

class USGPawnData;
/**
 * 
 */
UCLASS()
class SG_API USGExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USGExperienceDefinition(const FObjectInitializer& ObjectInitializer);

	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TObjectPtr<USGPawnData> DefaultPawnData;

	/** 단순히 마킹 및 기억용으로 남겨둔다. */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FString> GameFeaturesToEnable;
};
