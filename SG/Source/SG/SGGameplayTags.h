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


	/*** Input Tag */
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Weapon_Fire);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_CosmeticTest);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_QuickBar_Weapon_0);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_QuickBar_Weapon_1);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_QuickBar_Weapon_2);

	
	/** Event */
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Movement_WeaponFire);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Equip_Cosmetic);


	/** Cosmetic Tag */
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Body);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Body_1);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Body_2);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Body_3);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Body_4);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Female_Clothes_Top_1);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Female_Clothes_Bottoms_1);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Female_Hair_1);
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cosmetic_Female_Arm_1);


	/** Notify */
	SG_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Notify_Equip_Weapon_Sword);
}
