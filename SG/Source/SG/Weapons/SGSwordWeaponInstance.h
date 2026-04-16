// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Weapons/SGWeaponInstance.h"
#include "SGSwordWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGSwordWeaponInstance : public USGWeaponInstance
{
	GENERATED_BODY()

public:
	USGSwordWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
