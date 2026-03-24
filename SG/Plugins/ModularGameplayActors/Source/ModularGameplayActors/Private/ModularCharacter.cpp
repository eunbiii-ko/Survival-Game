// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularCharacter.h"

#include "Components/GameFrameworkComponentManager.h"

// Sets default values
AModularCharacter::AModularCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AModularCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	// Receiver로 등록한다.
	// 해당 캐릭터도 GameFeature에서 Add Component 또는 다른 Action을 수신하기 위함
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

// Called when the game starts or when spawned
void AModularCharacter::BeginPlay()
{
	// Lyra에서는 NAME_GameActorReady을 이용해서 여러 세팅을 한다. 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
		this,
		UGameFrameworkComponentManager::NAME_GameActorReady);
	
	Super::BeginPlay();
}

// Called every frame
void AModularCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AModularCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AModularCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}

