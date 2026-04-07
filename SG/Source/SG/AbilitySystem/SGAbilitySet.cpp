// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/SGAbilitySet.h"

#include "SGAbilitySystemComponent.h"
#include "Abilities/SGGameplayAbility.h"

void FSGAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FSGAbilitySet_GrantedHandles::TakeFromAbilitySystem(USGAbilitySystemComponent* SGASC)
{
	if (!SGASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			// ActivatableAbilities에서 제거한다.
			SGASC->ClearAbility(Handle);
		}
	}
}

////////////////////////////////////////////////////////////////////////////

USGAbilitySet::USGAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGAbilitySet::GiveToAbilitySystem(USGAbilitySystemComponent* SGASC,
	FSGAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(SGASC);

	if (!SGASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	int32 AbilityIndex = 0;
	// GrantedGameplayAbilities에 저장된 Ability를 부여한다.
	for (AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		// AbilityIndex번째 Ability를 가져온다. 
		const FSGAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		// GiveAbility()에서 만약 EGameplayAbilityInstancingPolicy::InstancedPerActor라면,
		// 내부적으로 Instance를 생성하지만 그렇지 않으면 CDO를 할당한다.
		// - 이를 통해 UObject 갯수를 줄여 UObject가 늘어남에 따라 늘어나는 성능적/메모리적 부하를 줄일 수 있다.
		USGGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<USGGameplayAbility>();

		// AbilitySpec은 GiveAbility로 전달되어, ActivatbleAbilities에 추가된다.
		FGameplayAbilitySpec AbilitySpec (AbilityCDO, AbilityToGrant.AbilityLevel);
		// Ability의 제공자 
		AbilitySpec.SourceObject = SourceObject;
		// Ability와 매핑시킬 InputTag
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = SGASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}
