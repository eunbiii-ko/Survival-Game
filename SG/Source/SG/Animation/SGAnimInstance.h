// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "SGAnimInstance.generated.h"

class UCharacterMovementComponent;

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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~ End of AnimInstance's interface

	//~ member methods
	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);
	//~ End of member methods

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComp;

	/** GameplayTag와 AnimInstance의 속성값을 매핑해준다. */
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	
	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Character State Data")
	float Direction = 0.f;

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
