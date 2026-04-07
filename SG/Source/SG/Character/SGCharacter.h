// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "SGCharacter.generated.h"

class USGCameraComponent;
class USGPawnExtensionComponent;

UCLASS()
class SG_API ASGCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ASGCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	//~ Begin IAbilitySystemInterface 
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface 
	
protected:
	virtual void BeginPlay() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SG | Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USGPawnExtensionComponent> PawnExtComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lyra | Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USGCameraComponent> CameraComp;
};
