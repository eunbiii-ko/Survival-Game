// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "SGAT_WaitInputStart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputStartDelegate);

/**
 * 어빌리티가 이미 활성화된 상태에서
 * 플레이어가 공격 버튼을 다시 눌렀는지 감지하는 AbilityTask
 */
UCLASS()
class SG_API USGAT_WaitInputStart : public UAbilityTask
{
	GENERATED_BODY()

public:
	USGAT_WaitInputStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta=(HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static USGAT_WaitInputStart* WaitInputStart(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;

public:
	UFUNCTION()
	void OnStartCallback();

public:
	UPROPERTY(BlueprintAssignable)
	FInputStartDelegate OnStart;

	
protected:
	FDelegateHandle DelegateHandle;
};
