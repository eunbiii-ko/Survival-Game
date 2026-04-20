// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class SG_API UAN_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
public:
	// 애니메이션 재생 중 특정 시점에 GameplayEvent를 발생시키는 Animation Notify
	// GAS(Gameplay Ability System)와 애니메이션을 연동하는 핵심 메커니즘
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	// Animation Notify의 이름을 동적으로 생성하여 반환
	// UE 에디터에서 애니메이션 노티파이를 볼 때 표시되는 이름을 커스터마이징
	virtual FString GetNotifyName_Implementation() const override;
	
	UPROPERTY(EditAnywhere, Category = "Gameplay Ability")
	FGameplayTag EventTag;
};
