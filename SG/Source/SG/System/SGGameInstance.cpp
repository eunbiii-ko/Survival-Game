// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/System/SGGameInstance.h"
#include "SG/SGGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

USGGameInstance::USGGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGGameInstance::Init()
{
	Super::Init();
	
	// Register our custom init states
	UGameFrameworkComponentManager* CompManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(CompManager))
	{
		CompManager->RegisterInitState(SGGameplayTags::InitState_Spawned, false, FGameplayTag());
		CompManager->RegisterInitState(SGGameplayTags::InitState_DataAvailable, false, SGGameplayTags::InitState_Spawned);
		CompManager->RegisterInitState(SGGameplayTags::InitState_DataInitialized, false, SGGameplayTags::InitState_DataAvailable);
		CompManager->RegisterInitState(SGGameplayTags::InitState_GameplayReady, false, SGGameplayTags::InitState_DataInitialized);
	}
}
