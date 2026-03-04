// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SGPlayerState.generated.h"

class USGPawnData;
class USGExperienceDefinition;
/**
 * 
 */
UCLASS()
class SG_API ASGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/**
	 * Actor's Interface
	 */
	virtual void PostInitializeComponents() override final;

	/**
	 * member methods
	 */
	void OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience);
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const USGPawnData* InPawnData);

private:
	/**
	 * member variable
	 */
	UPROPERTY()
	TObjectPtr<const USGPawnData> PawnData;
};
