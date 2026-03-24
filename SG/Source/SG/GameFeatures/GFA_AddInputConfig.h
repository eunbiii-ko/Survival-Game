// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SG/GameFeatures/GFA_WorldActionBase.h"
#include "GFA_AddInputConfig.generated.h"

struct FSGMappableConfigPair;
struct FComponentRequestHandle;
/**
 * 
 */
UCLASS()
class SG_API UGFA_AddInputConfig : public UGFA_WorldActionBase
{
	GENERATED_BODY()
public:
	/**
	 * UGameFeatureAction interface
	 */
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	
	/**
	 * UGFA_WorldActionBase interface
	 */
	virtual void AddToworld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;


private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	/** GameFeatureState가 변경될 때, 콜백함수
	 * (혹은 ExtensionEvent를 GameFrameworkComponentManager를 통해 호출 가능) */
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	/** InputConfig를 EnhancedInputLocalPlayerSubsystem에 추가 및 제거한다. */
	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);

	/** ActiveData를 초기화한다. */
	void Reset(FPerContextData& ActiveData);

	/** world -> (extension request, pawn) */
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/** PlayerMappableInputConfig를 들고 있는 멤버 변수 */
	UPROPERTY(EditAnywhere)
	TArray<FSGMappableConfigPair> InputConfigs;
};


