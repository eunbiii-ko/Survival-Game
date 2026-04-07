// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SGGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ESGAbilityActivationPolicy : uint8
{
	/** Input이 Trigger 되었을 경우 (Pressed/Released) */
	OnInputTriggered,
	/** Input이 Held 되어 있을 경우 */
	WhileInputActive,
	/** Avatar가 생성되었을 경우 */
	OnSpawn,
};

///////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS()
class SG_API USGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USGGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** GA가 언제 활성화될지 결정 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SG | AbilityActivation")
	ESGAbilityActivationPolicy ActivationPolicy;
};
