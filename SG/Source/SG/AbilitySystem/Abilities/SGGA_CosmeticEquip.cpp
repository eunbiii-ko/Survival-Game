// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/Abilities/SGGA_CosmeticEquip.h"

#include "GameFramework/Character.h"
#include "SG/SGGameplayTags.h"
#include "SG/Cosmetics/SGControllerComp_CharacterParts.h"

USGGA_CosmeticEquip::USGGA_CosmeticEquip(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

	ActivationPolicy = ESGAbilityActivationPolicy::OnInputTriggered;
}

void USGGA_CosmeticEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 서버에서만 실행한다. 
	if (!HasAuthority(&ActivationInfo))
	{
		return;
	}
	
	if (!NewCosmeticPart.PartClass)
	{
		K2_EndAbility();
		return;
	}

	APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get());
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

	// Cosmetic Mesh를 적용한다.
	CharacterPartControllerComp->ChangeCosmeticPart(SGGameplayTags::Cosmetic_Body, NewCosmeticPart);

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
