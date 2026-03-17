// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Input/SGInputConfig.h"

#include "SG/SGLogChannels.h"

USGInputConfig::USGInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UInputAction* USGInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// NativeInputActions을 순회하며, Input으로 들어온 InputTag가 있는지 체크한다:
	// - 있으면 그에따른 Input을 반환하고, 없으면 nullptr 반환
	for (const FSGInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogSG, Error, TEXT("[ULccInputConfig::FindNativeInputActionForTag] can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}

const UInputAction* USGInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// AbilityInputActions 순회하며, Input으로 들어온 InputTag가 있는지 체크한다:
	// - 있으면 그에따른 Input을 반환하고, 없으면 nullptr 반환
	for (const FSGInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogSG, Error, TEXT("[USGInputConfig::FindNativeInputActionForTag] can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
