// Fill out your copyright notice in the Description page of Project Settings.


#include "SGGameplayTags.h"

namespace SGGameplayTags
{
	/*** Init State */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "1: Actor/component has initially spawned and can be extended");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");


	/*** Input Tag */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Fire, "InputTag.Weapon.Fire", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_CosmeticTest, "InputTag.CosmeticTest", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_QuickBar_Weapon_0, "InputTag.QuickBar.Weapon.0", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_QuickBar_Weapon_1, "InputTag.QuickBar.Weapon.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_QuickBar_Weapon_2, "InputTag.QuickBar.Weapon.2", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_WeaponFire, "Event.Movement.WeaponFire", "");

	
	/** Cosmetic Tag */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Body, "Cosmetic.Body", "");
}