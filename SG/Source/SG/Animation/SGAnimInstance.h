// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "SGAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//~ AnimInstance's interface
	virtual void NativeInitializeAnimation() override;
	//~ End of AnimInstance's interface

	//~ member methods
	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);
	//~ End of member methods
	
	/** 해당 속성값은 Lyra의 AnimBP에서 사용되는 값이다. */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.f;

	/** GameplayTag와 AnimInstance의 속성값을 매핑해준다. */
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
