// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGCharacter.h"
#include "SG/Character/SGPawnExtensionComponent.h"

ASGCharacter::ASGCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PawnExtComp = CreateDefaultSubobject<USGPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

void ASGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

