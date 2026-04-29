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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Test_Z, "InputTag.Test.Z", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Test_X, "InputTag.Test.X", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Test_C, "InputTag.Test.C", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Test_V, "InputTag.Test.V", "");


	/** Event */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Movement_WeaponFire, "Event.Movement.WeaponFire", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Equip_Cosmetic, "Event.Equip.Cosmetic", "");

	
	/** Cosmetic Tag */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Body, "Cosmetic.Body", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Body_1, "Cosmetic.Body.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Body_2, "Cosmetic.Body.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Body_3, "Cosmetic.Body.3", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Body_4, "Cosmetic.Body.4", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Top, "Cosmetic.Female.Clothes.Top", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Top_1, "Cosmetic.Female.Clothes.Top.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Top_2, "Cosmetic.Female.Clothes.Top.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Top_3, "Cosmetic.Female.Clothes.Top.3", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Top_4, "Cosmetic.Female.Clothes.Top.4", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Bottoms, "Cosmetic.Female.Clothes.Bottoms", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Bottoms_1, "Cosmetic.Female.Clothes.Bottoms.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Clothes_Bottoms_2, "Cosmetic.Female.Clothes.Bottoms.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Hair_1, "Cosmetic.Female.Hair.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Hair, "Cosmetic.Female.Hair", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Foot, "Cosmetic.Female.Foot", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Foot_1, "Cosmetic.Female.Foot.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Foot_2, "Cosmetic.Female.Foot.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Hand, "Cosmetic.Female.Hand", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Hand_1, "Cosmetic.Female.Hand.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cosmetic_Female_Hand_2, "Cosmetic.Female.Hand.2", "");

	/** Notify */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Notify_Equip_Weapon_Sword, "Notify.Equip.Weapon.Sword", "");
}