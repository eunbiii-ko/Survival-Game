// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGGameMode.generated.h"

class USGExperienceDefinition;
/**
 * 
 */
UCLASS()
class SG_API ASGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGGameMode(const FObjectInitializer& ObjectInitializer);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override final;
	virtual void InitGameState() override final;


private:
	/**
	 * member methods 
	 */
	void OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience);
	
protected:
	/**
	* member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
};
