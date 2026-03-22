// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "SGExperienceManagerComponent.generated.h"

class USGExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSGExperienceLoaded, const USGExperienceDefinition*);

enum class ESGExperienceLoadState
{
	Unloaded,		// 로드 안됨
	Loading,		// 로드중
	LoadingGameFeatures, // GemeFeature 로딩중
	ExecutingActions,		// GameFeature 내 Action 실행 
	Loaded,			// 로드됨
	Deactivating,	// 로딩 해제
};



/**
 * SGExperienceManagerComponent
 * - GameState를 Owner로 가지면서, Experience의 상태 정보를 갖고 있는 Compoennt이다.
 * - 뿐만 아니라, Manager라는 단어가 포함되어 있듯이, Experience의 로딩 상태 업데이트 및 이벤트를 관리한다. 
 */
UCLASS()
class SG_API USGExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	USGExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override final;

	/** 현재 로딩이 완료가 되었고, CurrentExperience가 존재하면 true */
	bool IsExperienceLoaded() { return (LoadedState == ESGExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }
	
	/**
	 * 아래의 OnExperienceLoaded에 바인딩하거나, 이미 Experience 로딩이 완료되었다면 바로 호출한다.
	 */
	void CallOrRegister_OnExperienceLoaded(FOnSGExperienceLoaded::FDelegate&& Delegate);


	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceID);
	const USGExperienceDefinition* GetCurrentExperienceChecked() const;
	
private:
	UFUNCTION()
	void OnRep_CurrentExperience();
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const USGExperienceDefinition> CurrentExperience;

	/** Experience의 로딩 상태를 모니터링 */
	ESGExperienceLoadState LoadedState = ESGExperienceLoadState::Unloaded;

	/** Experience 로딩이 완료된 후, Broadcasting Delegate */
	FOnSGExperienceLoaded OnExperienceLoaded;

	/** 활성화된 GameFeature Plugins */
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
