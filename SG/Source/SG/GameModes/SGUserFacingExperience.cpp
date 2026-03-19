// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGUserFacingExperience.h"

#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* USGUserFacingExperience::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	// UCommonSession_HostSessionRequest는 UObject로 생성하였기 때문에 알아서 GC가 된다.
	// 해당 객체는 현재 프레임에서 사용하기 때문에 GC에 대한 염려가 필요없다.
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
