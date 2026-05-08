// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EWeaponHandType : uint8
{
	LeftHand,
	RightHand,
	TwoHand,

	Count	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed,
	OneHandSword,
	TwoHandSword,
	GreatSword,
	Shield,
	Staff,
	Bow,

	Count	UMETA(Hidden)
};