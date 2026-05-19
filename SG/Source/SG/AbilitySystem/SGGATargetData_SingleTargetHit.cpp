// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGGATargetData_SingleTargetHit.h"




bool FSGGATargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}

