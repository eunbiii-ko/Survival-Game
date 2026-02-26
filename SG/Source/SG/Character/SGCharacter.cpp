// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGCharacter.h"

ASGCharacter::ASGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

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

