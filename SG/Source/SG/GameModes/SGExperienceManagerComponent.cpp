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
	OnExperienceFullLoadComplete();
}

void USGExperienceManagerComponent::OnExperienceFullLoadComplete()
{
	check(LoadedState != ESGExperienceLoadState::Loaded);

	// 로드 완료 상태로 업데이트한다.
	LoadedState = ESGExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}
