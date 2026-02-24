// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	USGAssetManager();

	static USGAssetManager& Get();
	virtual void StartInitialLoading() override;
	
	/**
	 * FScopeLogTime을 사용할지, 안할지 유무 판단을 위해 사용한다.
	 */
	static bool ShouldLogAssetLoads();

	/**
	 * 에셋을 동기 방식으로 로딩한다.
	 */
	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	/**
	 * 정적 로딩으로 BP Class와 Object를 로딩한다.
	 */
	template <typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	template <typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	/**
	 * [THREAD-SAFE] 메모리에 로딩된 에셋을 캐싱한다.
	 */
	void AddLoadedAsset(const UObject* Asset);
	
	// GC의 대상
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Object 단위 Locking  
	FCriticalSection SyncObject;
};

template <typename AssetType>
AssetType* USGAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;
	// 에셋의 경로를 가져온다.
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	// 가져온 경로가 유효하면 
	if (AssetPath.IsValid())
	{
		// TSoftObjectPtr::Get()
		// - 에셋이 로딩되어 있으면 바로 반환
		// - 로딩되어 있지 않으면 nullptr 반환
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			// 로딩되어 있지 않으면 동기 로딩 진행
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// AddLoadedAsset(): 메모리에 존재하기 위한 장치라고 생각하면 된다.
			// 즉, 한 번 등록되면 직접 언로드하지 않는 이상 메모리에서 해제되지 않는다.(Unload되지 않는다)
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template <typename AssetType>
TSubclassOf<AssetType> USGAssetManager::GetSubclass(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;
	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();
	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}