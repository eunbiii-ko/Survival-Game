// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/Abilities/SGGA_CosmeticEquip.h"

#include "GameFramework/Character.h"
#include "SG/SGGameplayTags.h"
#include "SG/Cosmetics/SGControllerComp_CharacterParts.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"

USGGA_CosmeticEquip::USGGA_CosmeticEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	ActivationPolicy = ESGAbilityActivationPolicy::OnInputTriggered;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = SGGameplayTags::Event_Equip_Cosmetic;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);	
	}
}

void USGGA_CosmeticEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 서버에서만 실행한다. 
	if (!HasAuthority(&ActivationInfo) || !TriggerEventData)
	{
		K2_EndAbility();
		return;
	}

	APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!AvatarPawn)
	{
		K2_EndAbility();
		return;
	}

	AController* AvatarController = AvatarPawn->GetController();
	if (!AvatarController)
	{
		K2_EndAbility();
		return;
	}

	USGControllerComp_CharacterParts* CharacterPartControllerComp = AvatarController->FindComponentByClass<USGControllerComp_CharacterParts>();
	if (!CharacterPartControllerComp)
	{
		K2_EndAbility();
		return;
	}

	// InstigatorTags를 순회하며 Map에서 해당 Part를 찾아 장착
	for (const FGameplayTag& Tag : TriggerEventData->InstigatorTags)
	{
		if (const FSGCharacterPart* Part = CosmeticPartMap.Find(Tag))
		{
			// Cosmetic Mesh를 적용한다.
			CharacterPartControllerComp->ChangeCosmeticPart(Tag, *Part);
		}
	}


	ACharacter* Character = Cast<ACharacter>(AvatarPawn);
	if (!Character)
	{
		K2_EndAbility();
		return;
	}
	UAnimInstance* AnimInst = Character->GetMesh()->GetAnimInstance();
	if (!AnimInst)
	{
		K2_EndAbility();
		return;
	}
	AnimInst->LinkAnimClassLayers(CosmeticLayer);
	K2_EndAbility();
}