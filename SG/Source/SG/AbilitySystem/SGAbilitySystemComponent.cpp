// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/AbilitySystem/SGAbilitySystemComponent.h"

#include "Abilities/SGGameplayAbility.h"

USGAbilitySystemComponent::USGAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 허용된 GameplayAbilitySpec들을 순회한다.
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		// Ability가 존재하고, DynamicAbilityTags에 InputTag가 있을 경우
		if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
		{
			// Input&Held에 넣어 Ability 처리를 기다린다.
			InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
		}
	}
}

void USGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	// 허용된 GameplayAbilitySpec들을 순회한다.
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		// Ability가 존재하고, DynamicAbilityTags에 InputTag가 있을 경우
		if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
		{
			// Released에 추가하고, Held에서 제거한다.
			InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
			InputHeldSpecHandles.Remove(AbilitySpec.Handle);
		}
	}
}

void USGAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	// InputHeldSpecHandles: 누르는 동안 발동돼야 할 Ability들
	// InputHeldSpecHandles에 대해 Ability 처리를 위해 AbilitiesToActivate에 추가한다.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		// ActivatableAbilities의 Handle 값 비교를 통해 GameplayAbilitySpec을 반환한다.
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			// Ability가 존재하고, 활성화되지 않은 경우
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				// Ability의 CDO를 가져온다.
				const USGGameplayAbility* SGAbilityCDO = CastChecked<USGGameplayAbility>(AbilitySpec->Ability);

				// ActivationPolicy가 WhileInputActive 속성이면 활성화로 등록
				if (SGAbilityCDO->ActivationPolicy == ESGAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(SpecHandle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// 이미 Ability가 활성화되어 있을 경우, Input Event(InputPressed)만 호출한다.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const USGGameplayAbility* SGAbilityCDO = CastChecked<USGGameplayAbility>(AbilitySpec->Ability);

					// ActivationPolicy가 OnInputTriggered 속성이면 활성화로 등록한다.
					if (SGAbilityCDO->ActivationPolicy == ESGAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	// 등록된 AbilitiesToActivate를 한 번에 등록한다.
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		// BP의 Activate()를 실행한다.
		TryActivateAbility(AbilitySpecHandle);
	}

	// 이번 프레임에 Released되었다면, 관련 GameplayAbility를 처리한다.
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	// 이번 프레임이 끝났으므로, InputPressedSpecHandles와 InputReleasedSpecHandles를 초기화한다.
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}
