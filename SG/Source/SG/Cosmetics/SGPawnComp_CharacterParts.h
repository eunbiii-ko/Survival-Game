// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGCharacterPartTypes.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SG/Cosmetics/SGCosmeticAnimationTypes.h"
#include "SGPawnComp_CharacterParts.generated.h"

class USkeletalMeshComponent;


/** 인스턴스화된 Character Part 단위 */
USTRUCT()
struct FSGAppliedCharacterPartEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	/** Character Part의 정의 (사용한 메타 데이터) */
	UPROPERTY()
	FSGCharacterPart Part;

	/**
	 * 생성됐을 때 LccCharacterPartList에서 할당받은 Part 핸들값
	 *  - FLccControllerCharacterPartEntry의 Handle값과 값이 같아야 한다.
	 *    -> 같으면 같은 Part
	 * */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** 인스턴스화된 Character Part용 Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComp = nullptr;
};

//////////////////////////////////////////////////////////////////////

/** LccPawnComp_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
USTRUCT(BlueprintType)
struct FSGCharacterPartList : public FFastArraySerializer
{
	GENERATED_BODY()

	FSGCharacterPartList()
		: OwnerComp(nullptr)
	{}

	FSGCharacterPartList(USGPawnComp_CharacterParts* InOwnerComp)
		: OwnerComp(InOwnerComp)
	{}

	FSGCharacterPartHandle AddEntry(const FSGCharacterPart& NewPart);

private:
	bool SpawnActorForEntry(FSGAppliedCharacterPartEntry& Entry);

	
public:
	/** 현재 인스턴스화된 Character Part */
	UPROPERTY()
	TArray<FSGAppliedCharacterPartEntry> Entries;

	
	/** 해당 LccCharacterPartList의 Onwer인 PawnComp */
	UPROPERTY(NotReplicated)
	TObjectPtr<USGPawnComp_CharacterParts> OwnerComp;
	
	/** PartHandle의 값을 할당 및 관리하는 변수 */
	int32 PartHandleCounter = 0;
};

//////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS()
class SG_API USGPawnComp_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	USGPawnComp_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	FSGCharacterPartHandle AddCharacterPart(const FSGCharacterPart& NewPart);
	
	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;
	
private:
	/** 인스턴스화된 Character Parts */
	UPROPERTY(Replicated, Transient)
	FSGCharacterPartList CharacterPartList;

	/** 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditDefaultsOnly, Category = "Cosmetics", meta = (AllowPrivateAccess = true))
	FSGAnimBodyStyleSelectionSet BodyMeshes;
};



