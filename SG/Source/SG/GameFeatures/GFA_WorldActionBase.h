// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GFA_WorldActionBase.generated.h"

struct FGameFeatureStateChangeContext;
/**
 * 
 */
UCLASS()
class SG_API UGFA_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	/**
	 * UGameFeatureAction's interface
	 */
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

	/**
	 * interface 
	 */
	virtual void AddToworld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGFA_WorldActionBase::AddToWorld, );
};
