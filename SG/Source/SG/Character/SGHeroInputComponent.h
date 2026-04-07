// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Character/SGHeroComponent.h"
#include "SGHeroInputComponent.generated.h"

struct FSGMappableConfigPair;
struct FInputActionValue;

/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class SG_API USGHeroInputComponent : public USGHeroComponent
{
	GENERATED_BODY()

public:
	USGHeroInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;

	/** Extension Event 정의 */
	static const FName NAME_BindInputsNow;
	

	/**
	 * IGameFrameworkInitStateInterface
	 */
	virtual FName GetFeatureName() const override final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;


protected:
	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	
	/**
	 * member variables
	 */
	UPROPERTY(EditDefaultsOnly)
	TArray<FSGMappableConfigPair> DefaultInputConfigs;
};
