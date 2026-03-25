// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "SG/Cosmetics/SGCharacterPartTypes.h"
#include "SGControllerComp_CharacterParts.generated.h"

/** ControllerComp가 소유하는 Character Parts */
USTRUCT()
struct FSGControllerCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part에 대한 정의 (메타 데이터) */
	UPROPERTY(EditAnywhere)
	FSGCharacterPart Part;

	/**
	 * Character Part 핸들 (고유값)
	 *  - Controller가 Possess하고 있는 Pawn에서 생성한 Character Part 핸들값
	 * */
	FSGCharacterPartHandle Handle;
};

//////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class SG_API USGControllerComp_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()

public:
	USGControllerComp_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Cosmetic")
	TArray<FSGControllerCharacterPartEntry> CharacterParts;
};
