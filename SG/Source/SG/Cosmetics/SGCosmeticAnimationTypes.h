// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SGCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FSGAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	/** AnimLayer를 적용할 대상 SkeletalMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;

	/** Cosmetic Tag라고 생각하면 된다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
};

USTRUCT(BlueprintType)
struct FSGAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;
	
	/** AnimLayer 적용할 SkeletalMesh를 들고 있다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSGAnimBodyStyleSelectionEntry> MeshRules;

	/** 디폴트로 적용할 SkeletalMesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;
	
	/** Physics Asset은 하나로 통일한다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
};