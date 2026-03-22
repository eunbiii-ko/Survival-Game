// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGExperienceManagerComponent.h"
#include "SGExperienceDefinition.h"
#include "Net/UnrealNetwork.h"
#include "SG/System/SGAssetManager.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"

USGExperienceManagerComponent::USGExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void USGExperienceManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentExperience);
}

void USGExperienceManagerComponent::OnRep_CurrentExperience()
{
	StartExperienceLoad();
}

void USGExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnSGExperienceLoaded::FDelegate&& Delegate)
{
	// 이미 Experience가 로드된 경우, 바로 델리게이트를 호출한다.
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	// 만약 Experience가 아직 로드되지 않은 경우, 델리게이트를 등록(추가)한다.
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void USGExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceID)
{
	USGAssetManager& AssetManager = USGAssetManager::Get();
	check(AssetManager.IsValid());

	// ExperienceID의 경로를 가져와서 동기 방식으로 로딩하여 AssetClass에 저장한다.
	TSubclassOf<USGExperienceDefinition> AssetClass;
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceID);
	AssetClass = Cast<UClass>(AssetPath.TryLoad());

	// CDO 형태로 가져온다.
	const USGExperienceDefinition* Experience = GetDefault<USGExperienceDefinition>(AssetClass);
	check(Experience);
	check(CurrentExperience == nullptr);

	// CDO로 CurrentExperience를 설정한다.
	CurrentExperience = Experience;

	StartExperienceLoad();
}

const USGExperienceDefinition* USGExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadedState == ESGExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}


void USGExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadedState == ESGExperienceLoadState::Unloaded);

	LoadedState = ESGExperienceLoadState::Loading;

	USGAssetManager& AssetManager = USGAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load assets associated with the experience
	// 나중에 GameFeature를 사용하여, Experience에 바인딩된 GameFeature Plugin을 로딩할 Bundle 이름을 추가한다.
	// - Bundle이라는게 후일 우리가 로딩할 에셋의 카테고리 이름이라고 생각하면 된다.
	TArray<FName> BundlesToLoad;
	{
		// [중요] OwnerNetMode가 NM_Standalone이면? Client/Server 둘 다 로딩에 추가한다!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient); // "Client"
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer); // "Server"
		}
	}

	FStreamableDelegate OnAssetLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// ChangeBundleStateForPrimaryAssets(): 비동기 방식으로 로딩한다. 
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// 로딩이 완료되었으면, ExecuteDelegate를 통해 OnAssetLoadedDelegate를 호출한다.
		FStreamableHandle::ExecuteDelegate(OnAssetLoadedDelegate);
	}
	else
	{
		// 로딩이 완료되면 OnAssetLoadedDelegate를 호출하도록 바인딩한다.
		Handle->BindCompleteDelegate(OnAssetLoadedDelegate);
		// 로딩 과정에서 문제가 발생해 실패한다면 다시 로딩을 시작한다. 
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetLoadedDelegate]()
		{
			// ExecuteIfBound(): Delegate가 바인딩되어 있으면, Delegate를 호출한다.
			OnAssetLoadedDelegate.ExecuteIfBound();
		}));
	}

	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void USGExperienceManagerComponent::OnExperienceLoadComplete()
{
	static int32 OnExperienceLoadCompleteFrameNumber = GFrameNumber;

	check(LoadedState == ESGExperienceLoadState::Loading);
	check(CurrentExperience);

	// 이전 활성화된 GameFeature Plugin의 URL을 초기화한다.
	GameFeaturePluginURLs.Reset();

	// 람다식을 만들고, 저장해둔다.
	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
	{
		// FeaturePluginList를 순회하며,
		// PluginURL을 ExperienceManagerComp의 GameFeaturePluginURLs에 추가한다.
		for (const FString& PluginName : FeaturePluginList)
		{
			FString PluginURL;
			// GetPluginURLByName(): 플러그인 이름(PluginName)을 넣으면 플러그인의 URL(PluginURL)을 반환한다.
			if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
			{
				This->GameFeaturePluginURLs.AddUnique(PluginURL);
			}
		}
	};

	// 일단 GameFeaturePluginURLs에 있는 Plugin만 활성화할 GameFeature Plugin 후보군으로 등록한다.
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// GameFeaturePluginURLs에 등록된 Plugin을 로딩 및 활성화한다.
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadedState = ESGExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// LoadAndActivateGameFeaturePlugin(): GameFeature 로딩과 동시에 활성화까지 해준다.
			// -> GameFeature 활성화가 되었을 때 어떤 함수를 부를지 델리게이트 설정이 가능하다. 
			// 매 Plugin이 로딩 및 활성화 이후,
			// OnGameFeaturePluginLoadComplete() 콜백 함수를 등록한다.
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL,
				FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		// 해당 함수가 불리는 것은 앞서 보았던 StreamableDelegateDelayHelper에 의해 불린다.
		OnExperienceFullLoadComplete();
	}
}

void USGExperienceManagerComponent::OnExperienceFullLoadComplete()
{
	check(LoadedState != ESGExperienceLoadState::Loaded);

	// 로드 완료 상태로 업데이트한다.
	LoadedState = ESGExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

void USGExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// 매 GameFeature Plugin이 로딩될 때, 해당 함수가 콜백으로 불린다.
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// GameFeaturePlugin 로딩이 다 끝났을 경우,
		// 기존 Loaded로서 OnExperienceFullLoadComplete()를 호출한다.
		OnExperienceFullLoadComplete();
	}
}
