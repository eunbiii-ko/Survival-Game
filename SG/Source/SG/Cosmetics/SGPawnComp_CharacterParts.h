// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGCharacterPartTypes.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "SG/Cosmetics/SGCosmeticAnimationTypes.h"
#include "SGPawnComp_CharacterParts.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSGSpawnedCharacterPartsChanged, USGPawnComp_CharacterParts*, ComponentWithChangedParts);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSGSpawnedCharacterPart);

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
	UPROPERTY(NotReplicated)
	int32 PartHandle = INDEX_NONE;

	/** 인스턴스화된 Character Part용 Actor */
	UPROPERTY(NotReplicated)
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
	{
	}

	FSGCharacterPartList(USGPawnComp_CharacterParts* InOwnerComp)
		: OwnerComp(InOwnerComp)
	{}

	FSGCharacterPartHandle AddEntry(const FSGCharacterPart& NewPart);
	void RemoveEntry(FSGCharacterPartHandle Handle);
	
	FGameplayTagContainer CollectCombinedTags() const;

public:
	// ~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	// ~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSGAppliedCharacterPartEntry, FSGCharacterPartList>(Entries, DeltaParms, *this);
	}


	
private:
	bool SpawnActorForEntry(FSGAppliedCharacterPartEntry& Entry);
	bool DestroyActorForEntry(FSGAppliedCharacterPartEntry& Entry);
	
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

template<>
struct TStructOpsTypeTraits<FSGCharacterPartList> : public TStructOpsTypeTraitsBase2<FSGCharacterPartList>
{
	enum { WithNetDeltaSerializer = true };
};


//////////////////////////////////////////////////////////////////////

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class SG_API USGPawnComp_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	USGPawnComp_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnRegister() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	FSGCharacterPartHandle AddCharacterPart(const FSGCharacterPart& NewPart);
	void RemoveCharacterPart(FSGCharacterPartHandle Handle);
	
	void BroadcastChanged(const FGameplayTagContainer& InTags = FGameplayTagContainer());
	
	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Cosmetics")
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	UPROPERTY(BlueprintAssignable, Category=Cosmetics, BlueprintCallable)
	FSGSpawnedCharacterPartsChanged OnCharacterPartsChanged;

	/** BlueprintAssignable은 BP상에서 할당 가능한 변수로 정의한다. */
	UPROPERTY(BlueprintAssignable)
	FSGSpawnedCharacterPart OnReadyForEquipWeapon;


	// 별도의 BP 구현 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BP_OnEquipWeapon();

	
private:
	// 델리게이트 핸들러 (클라이언트에서 호출됨)
	UFUNCTION()
	void OnReadyForEquipWeaponHandler();

	// Server RPC (서버에서 실행됨)
	UFUNCTION(Server, Reliable)
	void ServerStartEquipWeapon();
	
	void EnsureSubMeshComponents();
private:
	/** 인스턴스화된 Character Parts */
	UPROPERTY(Replicated, Transient, meta = (AllowPrivateAccess = true))
	FSGCharacterPartList CharacterPartList;

	/** 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditDefaultsOnly, Category = "Cosmetics", meta = (AllowPrivateAccess = true))
	FSGAnimBodyStyleSelectionSet BodyMeshes;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(Transient)
	USkeletalMeshComponent* TopMeshComp = nullptr;

	UPROPERTY(Transient)
	USkeletalMeshComponent* BottomMeshComp = nullptr;

	UPROPERTY(Transient)
	USkeletalMeshComponent* HandMeshComp = nullptr;

	UPROPERTY(Transient)
	USkeletalMeshComponent* FootMeshComp = nullptr;
};



