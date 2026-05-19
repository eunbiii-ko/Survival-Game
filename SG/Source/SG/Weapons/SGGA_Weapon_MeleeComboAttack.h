// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/Weapons/SGGA_Weapon_MeleeAttack.h"
#include "SGGA_Weapon_MeleeComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGGA_Weapon_MeleeComboAttack : public USGGA_Weapon_MeleeAttack
{
	GENERATED_BODY()

public:
	USGGA_Weapon_MeleeComboAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void HandleMontageEvent(FGameplayEventData Payload) override;
	
private:
	void WaitInputContinue();
	void WaitInputStop();
	
	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UFUNCTION()
	void OnInputStart();

	UFUNCTION()
	void OnInputCancel();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "SG | Melee Combo Attack")
	TSubclassOf<USGGA_Weapon_MeleeComboAttack> NextAbilityClass;

private:
	bool bInputPressed = false;
	bool bInputReleased = false;
	
};
