// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGCharacterPartTypes.generated.h"

USTRUCT(BlueprintType)
struct FSGCharacterPart
{
	GENERATED_BODY()

	/**
	 * 파츠로 생성된 Actor의 Class 정보를 갖는다.
	 *  -> Actor BP를 하나의 파츠라고 생각하면 된다.
	 * */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	/**
	 * Actor를 소환하여 붙이는데 특정 Bone에 붙이기 위한 정보
	 * (따로 정의되어 있지 않으면 Root에 붙인다.)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};

//////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FSGCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}
	
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
};