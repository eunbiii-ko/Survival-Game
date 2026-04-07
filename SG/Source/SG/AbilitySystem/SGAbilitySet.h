// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "SGAbilitySet.generated.h"

struct FGameplayAbilitySpecHandle;
class USGAbilitySystemComponent;
class USGGameplayAbility;

////////////////////////////////////////////////////////////////////////////

/**
 * GameplayAbility의 Wrapper 클래스
 * - 추가적인 커스터마이징 가능
 */
USTRUCT(BlueprintType)
struct FSGAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	/** 허용된 GameplayAbility */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USGGameplayAbility> Ability = nullptr;

	/** Input 처리를 위한 GameplayTag */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	/** Ability의 허용 조건 (Level) */
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

////////////////////////////////////////////////////////////////////////////

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSGAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(USGAbilitySystemComponent* SGASC);
	
protected:
	/** 허용된 GameplayAbilitySpecHandle(int32) */
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

////////////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS()
class SG_API USGAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USGAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** ASC에 허용가능한 Ability를 추가한다. */
	void GiveToAbilitySystem(USGAbilitySystemComponent* SGASC, FSGAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FSGAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
