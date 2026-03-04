// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGGameMode.generated.h"

class USGPawnData;
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
	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override final;
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override final;
	APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override final;
	const USGPawnData* GetPawnDataForController(const AController* InController) const;

	
private:
	/**
	 * member methods 
	 */
	void OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceID);
	
protected:
	/**
	* member methods
	*/
	void HandleMatchAssignmentIfNotExpectingOne();
};
