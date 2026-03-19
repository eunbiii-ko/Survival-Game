// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonSessionSubsystem.h"
#include "Engine/AssetManager.h"
#include "SG/Player/SGPlayerController.h"

//////////////////////////////////////////////////////////////////////////
// UCommonSession_HostSessionRequest
//////////////////////////////////////////////////////////////////////////
FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager, MapID를 통해 FAssetData를 반환하고,
	// 이를 통해 PackageName를 통해 Map 경로를 반환한다.
	UE_LOG(LogTemp, Warning, TEXT("GetMapName called with MapID: %s"), *MapID.ToString());

	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData))
	{
		UE_LOG(LogTemp, Warning, TEXT("MapAssetData found: %s"), *MapAssetData.PackageName.ToString());
		return MapAssetData.PackageName.ToString();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get map data for MapID: %s"), *MapID.ToString());
	}
	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		// ?를 separate로 복수개의 ExtraArg를 추가한다.
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	// Map 경로 앞에 추가하여, 최종 TravelURL을 생성한다.
	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

//////////////////////////////////////////////////////////////////////////
// UCommonSessionSubsystem
//////////////////////////////////////////////////////////////////////////
void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	if (!HostingPlayer || !Request)
	{
		return;
	}

	FString TravelURL = Request->ConstructTravelURL();
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	// 서버 또는 Standalone이면 직접 ServerTravel
	if (World->GetNetMode() != NM_Client)
	{
		World->ServerTravel(TravelURL);
	}
	else
	{
		// 클라이언트면 서버에 RPC 요청
		if (ASGPlayerController* PC = Cast<ASGPlayerController>(HostingPlayer))
		{
			PC->ServerRequestMapTravel(TravelURL);
		}
	}
}


