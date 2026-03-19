// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"


/**
 * UCommonSession_HostSessionRequest을 만들어 CommonSessionSubsystem에 전달하면,
 * CommonSessionSubsystem에서 MapID와 CmdArgs로 최종적인 CmdArgs를 만들어 전달한다.
 */
UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	/** MapID -> Text 변환 */
	FString GetMapName() const;

	/** ServerTravel에 전달할 최종 URL을 생성한다. */
	FString ConstructTravelURL() const;

	/** 준비할 Map ID (맵 경로) */
	UPROPERTY(BlueprintReadWrite, Category = "Session")
	FPrimaryAssetId MapID;

	/** 전달할 CmdArgs (Experience 이름) */
	UPROPERTY(BlueprintReadWrite, Category = "Session")
	TMap<FString, FString> ExtraArgs;
};


/**
 * UCommonSessionSubsystem은 LCC에서 사용돼야 하기 때문에,
 * Module Export를 해줘야 하고, 그래서 COMMONUSER_API를 추가해줘야 한다.
 */
UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UCommonSessionSubsystem() {}

	UFUNCTION(BlueprintCallable, Category = "Session")
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);


	
	/**
	 * member variables
	 */
	/** 맵의 경로 */
	FString PendingTravelURL;
};
