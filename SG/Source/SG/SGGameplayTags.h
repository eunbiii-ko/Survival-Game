// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace SGGameplayTags
{
	/*** Init State */
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);
}
