// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "SGPlayerController.generated.h"

class USGAbilitySystemComponent;
class ASGPlayerState;


/**
 * 
 */
UCLASS()
class SG_API ASGPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ASGPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerRequestMapTravel(const FString& TravelURL);

	//~ Begin APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override final;
	//~ End APlayerController interface

	//~ Begin member methods
	ASGPlayerState* GetSGPlayerState() const;
	USGAbilitySystemComponent* GetSGAbilitySystemComponent() const;
	//~ End member methods
};
