// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGPawnData.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USGPawnData(const FObjectInitializer& ObjectInitializer);

	/** Pawn의 Class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SG | PawnData")
	TSubclassOf<APawn> PawnClass;
};
