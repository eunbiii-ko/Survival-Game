// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SGTaggedActor.generated.h"

UCLASS()
class SG_API ASGTaggedActor : public AActor, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:	
	ASGTaggedActor();
	virtual void Tick(float DeltaTime) override;

	//~IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	//~End of IGameplayTagAssetInterface
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CosmeticTag;

protected:
	// Gameplay-related tags associated with this actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Actor)
	FGameplayTagContainer StaticGameplayTags;
};
