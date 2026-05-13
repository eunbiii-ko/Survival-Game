// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UANS_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UANS_SendGameplayEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag BeginEventTag;

	UPROPERTY(EditAnywhere)
	FGameplayTag TickEventTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag EndEventTag;

	UPROPERTY(EditAnywhere)
	FGameplayEventData EventData;
};
