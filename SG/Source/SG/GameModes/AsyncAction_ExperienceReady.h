// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

class USGExperienceDefinition;

/** BP를 통한 UFUNCTION으로 바인딩하는 Multicast Delegate: Dynamic */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

/**
 * 
 */
UCLASS()
class SG_API UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출 */
	UFUNCTION(BlueprintCallable, meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly="true"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady2(UObject* WorldContextObject);

	/**
	 * UBlueprintAsyncActionBase interface
	 */
	virtual void Activate() override;

	/**
	 * Step1 - Step4
	 */
	// GameState 가져온다.
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	// Experience Loading 바인딩
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	// Experience Loading 완료 여부 체크
	void Step3_HandleExperienceLoaded(const USGExperienceDefinition* CurrentExperience);
	// 델리게이트 브로드캐스트 
	void Step4_BroadcastReady();

	
	/** BlueprintAssignable은 BP상에서 할당 가능한 변수로 정의한다. */
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;
	
	/** WorldPtr은 단순 로직상 캐싱하는 용도 */
	TWeakObjectPtr<UWorld> WorldPtr;
};
