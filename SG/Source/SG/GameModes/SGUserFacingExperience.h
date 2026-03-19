// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SGUserFacingExperience.generated.h"

/**
 * 
 */
UCLASS()
class SG_API USGUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** Map 로딩 및 Experience 전환을 위해,
	 * MapID와 ExperienceID를 활용하여 HostSessionRequest를 생성한다.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;


	
	/** the specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Experience",
		meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** the gameplay experience to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Experience",
		meta = (AllowedTypes = "SGExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
};
