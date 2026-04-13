// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SGTaggedActor.generated.h"

UCLASS()
class SG_API ASGTaggedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASGTaggedActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CosmeticTag;
};
