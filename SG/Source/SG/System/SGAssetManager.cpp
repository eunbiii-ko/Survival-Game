// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/System/SGAssetManager.h"
#include "SG/SGLogChannels.h"

USGAssetManager::USGAssetManager()
{
}

USGAssetManager& USGAssetManager::Get()
{
	check(GEngine);

	// AssetManager를 UEngine의 GEngine의 AssetManager의 Class를 오버라이드 했기 때문에,
	// GEngine에 AssetManager가 있다.
	if (USGAssetManager* Singleton = Cast<USGAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogSG, Fatal, TEXT("Cannot use AssetManager if no AssetManagerClassName is defined!"));

	// 위 UE_LOG()의 Fatal로 인해, Crash가 나기 때문에 도달하지 않지만 컴파일을 위해 더미를 리턴한다. 
	return *NewObject<USGAssetManager>();
}

void USGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

bool USGAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	// "LogAssetLoads"가 존재하면 true, 존재하지 않으면 false 
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

UObject* USGAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 해당 함수를 따로 만든 이유는 synchronous load asset이 불필요하게 하는 것이 없는지 확인하기 위함이다.
	if (AssetPath.IsValid())
	{
		// FScopeLogTime을 확인한다.
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			// 단순히 로깅하면서, 초단위로 로깅을 진행한다.
			// 함수 실행이 얼마나 걸리는지 체크할 수 있다.
			// 즉, 동기 로딩에 몇 초가 걸리는지 확인할 수 있다. 
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchronous loaded assets [%s]"),
				*AssetPath.ToString()),
				nullptr,
				FScopeLogTime::ScopeLog_Seconds);
		}

		// AssetManager가 있으면, AssetManager의 StreamableManager를 통해 정적 로딩
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// AssetManager가 없으면 (엔진에서 AssetManager가 아직 안만들어졌으면),
		// FSoftObjectPath의 TryLoad()를 통해 정적 로딩
		// - 내부적으로 StaticLoadObject() 호출 전, 에셋이 로딩되었는지 확인(StaticFindObject())하고 없으면 로딩한다.
		return AssetPath.TryLoad();
	}

	return nullptr;
}

void USGAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}
