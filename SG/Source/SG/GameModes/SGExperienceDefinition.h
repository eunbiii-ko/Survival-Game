// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGExperienceDefinition.generated.h"

class USGExperienceActionSet;
class USGPawnData;
class UGameFeatureAction;
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

	/** Gameplay 용도에 맞게 분류의 목적으로 사용한다. */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TObjectPtr<USGExperienceActionSet>> ActionSets;

	/** 일반적인 GameFeatureAction으로서 추가 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
