// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGGameState.generated.h"

class USGExperienceManagerComponent;
/**
 * 
 */
UCLASS()
class SG_API ASGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASGGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY()
	TObjectPtr<USGExperienceManagerComponent> ExperienceManagerComp;
};
