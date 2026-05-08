// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Equipment/SGGA_FromEquipment.h"
#include "SGGA_Weapon_Attack.generated.h"

class ASGCharacter;
/**
 * 
 */
UCLASS()
class SG_API USGGA_Weapon_Attack : public USGGA_FromEquipment
{
	GENERATED_BODY()

public:
	USGGA_Weapon_Attack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void ParseTargetData(const FGameplayAbilityTargetDataHandle& InTargetDataHandle, TArray<int32>& OutCharacterHitIndexes, TArray<int32>& OutBlockHitIndexes);

	bool IsCharacterBlockingHit(ASGCharacter* TargetCharacter);
	
protected:
	float BlockingAngle;
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> CachedHitActors;
};
