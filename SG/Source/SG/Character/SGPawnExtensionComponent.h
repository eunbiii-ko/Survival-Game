// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "SGPawnExtensionComponent.generated.h"

/**
 * 컴포넌트들의 초기화를 담당한다.
 */
UCLASS()
class SG_API USGPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	USGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;
	
	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override final;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override final;
	virtual void CheckDefaultInitialization() override final;
	//~ End IGameFrameworkInitStateInterface interface
	
protected:
	virtual void OnRegister() override final;
	virtual void BeginPlay() override final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

	
};


