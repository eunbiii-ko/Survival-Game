// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

/** Enhanced Input System의 UPlayerMappableInputConfig를 관리하기 위한 구조체 */
USTRUCT()
struct FSGMappableConfigPair
{
	GENERATED_BODY()
	
public:
	FSGMappableConfigPair() = default;

	/** IMC 에셋에 대한 참조 */
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	/** 자동 활성화 여부 */
	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};