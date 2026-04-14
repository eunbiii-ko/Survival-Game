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
	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float GroundDistance = -1.f;

	/** GameplayTag와 AnimInstance의 속성값을 매핑해준다. */
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float AnimPitch = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float Direction = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float LeftClavicleAnimMultiplier = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float RightClavicleAnimMultiplier = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float LeftClavicleAnimAdditive = 0.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float RightClavicleAnimAdditive = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float Rotation = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	bool bIsFalling = false;
};
