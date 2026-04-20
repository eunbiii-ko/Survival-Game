// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Cosmetics/SGPawnComp_CharacterParts.h"

#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "SG/SGLogChannels.h"


FSGCharacterPartHandle FSGCharacterPartList::AddEntry(const FSGCharacterPart& NewPart)
{
	// PawnComp의 CharacterPartList가 PartHandle을 관리하고,
	// 이를 ControllerComp_CharacterParts에 전달한다.
	FSGCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	// 서버라면, AppliedCharacterPartEntry를 Entries에 추가한다.
	if (ensure(OwnerComp && OwnerComp->GetOwner()) && OwnerComp->GetOwner()->HasAuthority())
	{
		FSGAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart; // 메타 데이터
		NewEntry.PartHandle = Result.PartHandle;
 
		// NewEntry를 이용해서 실제 Actor를 생성하고,
		// OwmerComp의 Owner Actor에 Actor끼리 RootComp로 Attach 시킨다.
		if (SpawnActorForEntry(NewEntry))
		{
			OwnerComp->BroadcastChanged();
			
		}

		MarkItemDirty(NewEntry);	
	}

	return Result;
}

void FSGCharacterPartList::RemoveEntry(FSGCharacterPartHandle Handle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FSGAppliedCharacterPartEntry& Entry = *EntryIt;
		if (Entry.PartHandle == Handle.PartHandle)
		{
			const bool bDestroyedActor = DestroyActorForEntry(Entry);
			EntryIt.RemoveCurrent();
			MarkArrayDirty();

			if (bDestroyedActor && ensure(OwnerComp))
			{
				OwnerComp->BroadcastChanged();
			}

			break;
		}
	}
}

FGameplayTagContainer FSGCharacterPartList::CollectCombinedTags() const
{
	FGameplayTagContainer Result;

	// Entries를 순회하며
	for (const FSGAppliedCharacterPartEntry& Entry : Entries)
	{
		// Part Actor가 생성되어 SpawnedComp에 캐싱되어 있으면
		if (Entry.SpawnedComp)
		{
			// 해당 Actor의 IGameplayTagAssetInterface를 통해 GameplayTag를 검색한다.
			// - 현재 TaggedActor는 IGameplayTagAssetInterface를 상속받지 않으므로 그냥 넘어간다.
			// - 나중에 각 Part에 대해 GameplayTag를 넣고 싶다면 이것을 상속받아 정의해야 한다.
			//		- 예를 들어, 특정 Lv100 이상 장착 가능한 장비를 만들고 싶다면 넣어야 한다.
			if (IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Entry.SpawnedComp->GetChildActor()))
			{
				TagInterface->GetOwnedGameplayTags(Result);
			}
		}
	}

	return Result;
}

void FSGCharacterPartList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	bool bDestroyedAnyActors = false;
	for (int32 Index : RemovedIndices)
	{
		FSGAppliedCharacterPartEntry& Entry = Entries[Index];
		bDestroyedAnyActors |= DestroyActorForEntry(Entry);
	}

	if (bDestroyedAnyActors && ensure(OwnerComp))
	{
		OwnerComp->BroadcastChanged();
	}
}

void FSGCharacterPartList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	bool bCreatedAnyActors = false;
	for (int32 Index : AddedIndices)
	{
		FSGAppliedCharacterPartEntry& Entry = Entries[Index];
		bCreatedAnyActors |= SpawnActorForEntry(Entry);
	}

	if (bCreatedAnyActors && ensure(OwnerComp))
	{
		OwnerComp->BroadcastChanged();
	}
}

void FSGCharacterPartList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	bool bChangedAnyActors = false;

	// We don't support dealing with propagating changes, just destroy and recreate
	for (int32 Index : ChangedIndices)
	{
		FSGAppliedCharacterPartEntry& Entry = Entries[Index];

		bChangedAnyActors |= DestroyActorForEntry(Entry);
		bChangedAnyActors |= SpawnActorForEntry(Entry);
	}

	if (bChangedAnyActors && ensure(OwnerComp))
	{
		OwnerComp->BroadcastChanged();
	}
}

bool FSGCharacterPartList::SpawnActorForEntry(FSGAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;

	if (!OwnerComp)
	{
		return bCreatedAnyActor;
	}
	UE_LOG(LogSG, Display, TEXT("[메시 장착] FSGCharacterPartList::SpawnActorForEntry() in %d netmode %d return %d"),
					OwnerComp->GetOwner()->HasAuthority(), (int32)OwnerComp->GetNetMode(), bCreatedAnyActor);
	
	if (!OwnerComp->IsNetMode(NM_DedicatedServer))
	//if (!OwnerComp->GetOwner()->HasAuthority())
	{
		// 전달된 AppliedCharacterPartEntry의 Part Class가 제대로 세팅되어 있다면
		if (Entry.Part.PartClass != nullptr)
		{
			// OwnerComp의 Owner에 속한 World를 반환한다.
			UWorld* World = OwnerComp->GetWorld();

			// LccPawnComp_CharacterParts에 어느 Comp에 붙일 것인지 결정한다.
			// OwnerComp->GetSceneComponentToAttachTo(): Pawn의 SceneComp를 가져온다. 
			if (USceneComponent* CompToAttachTo = OwnerComp->GetSceneComponentToAttachTo())
			{
				// 붙일 Comp인 CompToAttachTo의 Bone 혹은 Socket Name을 통해
				// 어디에 붙일지 Transform을 계산한다.
				const FTransform SpawnTransform = CompToAttachTo->GetSocketTransform(Entry.Part.SocketName);

				// Actor-Actor 결합이므로, ChildActorComponent를 활용한다.
				// OwnerComp->GetOwner(): Part를 장착할 Pawn
				UChildActorComponent* PartComp = NewObject<UChildActorComponent>(OwnerComp->GetOwner());
				PartComp->SetupAttachment(CompToAttachTo, Entry.Part.SocketName);
				// UChildActorComponent는 Actor의 메타 클래스를 넘겨주고,
				// RegisterComponent()를 호출하면 스폰까지 진행된다.
				PartComp->SetChildActorClass(Entry.Part.PartClass);
				// RenderWorld인 FScene에 변경 내용을 전달한다. (혹은 생성한다.)
				PartComp->RegisterComponent();

				// ChildActorComp에서 생성한 Actor를 반환한다.
				// PartComp->GetChildActor(): Entry.Part.PartClass로 소환된 Actor
				if (AActor* SpawnedActor = PartComp->GetChildActor())
				{
					// 해당 Actor가 Parent인 LccPawnComp_CharacterParts의 Owner Actor보다
					// 먼저 Tick이 실행되지 않도록 선행조건을 붙인다.
					if (USceneComponent* SpawnedRootComp = SpawnedActor->GetRootComponent())
					{
						// Tick 순서: CompToAttachTo(Owner, 부모) -> SpawnedActor(PartComp, 자식)
						SpawnedRootComp->AddTickPrerequisiteComponent(CompToAttachTo);
					}
				}
			
				// 실제 스폰한 Actor
				Entry.SpawnedComp = PartComp;
				bCreatedAnyActor = true;
				OwnerComp->OnReadyForEquipWeapon.Broadcast();
				
				UE_LOG(LogSG, Display, TEXT("[메시 장착] FSGCharacterPartList::SpawnActorForEntry() in %d return %d"),
					OwnerComp->GetOwner()->HasAuthority(), bCreatedAnyActor);
			}
		}
	}
	
	return bCreatedAnyActor;
}

bool FSGCharacterPartList::DestroyActorForEntry(FSGAppliedCharacterPartEntry& Entry)
{
	bool bDestroyedAnyActors = false;

	if (Entry.SpawnedComp != nullptr)
	{
		Entry.SpawnedComp->DestroyComponent();
		Entry.SpawnedComp = nullptr;
		bDestroyedAnyActors = true;
	}

	return bDestroyedAnyActors;
}

//////////////////////////////////////////////////////////////////////

USGPawnComp_CharacterParts::USGPawnComp_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), CharacterPartList(this)
{
	SetIsReplicatedByDefault(true);
}

void USGPawnComp_CharacterParts::OnRegister()
{
	Super::OnRegister();

	if (!IsTemplate())
	{
		CharacterPartList.OwnerComp = this;
	}

	OnReadyForEquipWeapon.AddDynamic(this, &ThisClass::OnReadyForEquipWeaponHandler);
}

void USGPawnComp_CharacterParts::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CharacterPartList);
}

FSGCharacterPartHandle USGPawnComp_CharacterParts::AddCharacterPart(const FSGCharacterPart& NewPart)
{
	return CharacterPartList.AddEntry(NewPart);
}

void USGPawnComp_CharacterParts::RemoveCharacterPart(FSGCharacterPartHandle Handle)
{
	CharacterPartList.RemoveEntry(Handle);
}

void USGPawnComp_CharacterParts::BroadcastChanged()
{
	const bool bReinitPose = true;

	// 현재 Owner의 SkeletalMeshComp를 가져온다.
	if (USkeletalMeshComponent* MeshComp = GetParentMeshComponent())
	{
		// BodyMeshes를 통해, GameplayTag를 활용하여 Tag에 맞은 SkeletalMesh로 재설정한다.
		// (Tag에 맞는 Mesh가 업으면 DefaultMesh를 반환한다.)
		// Tag를 가져와서
		const FGameplayTagContainer MergedTags = GetCombinedTags(FGameplayTag());
		// Tag에 맞는 Mesh를 가져온다.
		USkeletalMesh* DesiredMesh = BodyMeshes.SelectBestBodyStyle(MergedTags);

		// SkeletalMesh를 초기화 및 Animation 초기화 시켜준다.
		MeshComp->SetSkeletalMesh(DesiredMesh, bReinitPose);
		MeshComp->LinkAnimClassLayers(Layer);
		UE_LOG(LogSG, Display, TEXT("USGPawnComp_CharacterParts::BroadcastChanged() in %d"), GetOwner()->HasAuthority());
		
		// PhysicsAsset을 초기화한다.
		if (UPhysicsAsset* PhysicsAsset = BodyMeshes.ForcedPhysicsAsset)
		{
			MeshComp->SetPhysicsAsset(PhysicsAsset, bReinitPose);
		}
	}

	OnCharacterPartsChanged.Broadcast(this);
}

USkeletalMeshComponent* USGPawnComp_CharacterParts::GetParentMeshComponent() const
{
	// Character를 활용하여, 최상위 SkeletalMesh를 반환한다.
	if (AActor* OwnerActor = GetOwner())
	{
		if (ACharacter* OwningCharacter = Cast<ACharacter>(OwnerActor))
		{
			if (USkeletalMeshComponent* MeshComp = OwningCharacter->GetMesh())
			{
				return MeshComp;
			}
		}
	}

	return nullptr;
}

USceneComponent* USGPawnComp_CharacterParts::GetSceneComponentToAttachTo() const
{
	// Parent에 SkeletalMeshComp가 있으면 반환한다.
	if (USkeletalMeshComponent* MeshComp = GetParentMeshComponent())
	{
		return MeshComp;
	}
	// 없으면 RootComponent을 확인한다.
	if (AActor* OwnerActor = GetOwner())
	{
		return OwnerActor->GetRootComponent();
	}
	
	// 없으면 nullptr 반환.
	return nullptr;
}

FGameplayTagContainer USGPawnComp_CharacterParts::GetCombinedTags(FGameplayTag RequiredPrefix) const
{
	// 현재 장착된 CharacterPartList의 Merge된(인스턴스화된) Tags를 모두 반환한다.
	FGameplayTagContainer Result = CharacterPartList.CollectCombinedTags();
	if (RequiredPrefix.IsValid())
	{
		// 만약 GameplayTag를 통해 필터링할 경우, 필터링해서 진행한다.
		return Result.Filter(FGameplayTagContainer(RequiredPrefix));
	}
	else
	{
		// 필터링할 GameplayTag가 없으면 그냥 반환한다.
		return Result;
	}
}

void USGPawnComp_CharacterParts::OnReadyForEquipWeaponHandler()
{
	ServerStartEquipWeapon();
	
}

void USGPawnComp_CharacterParts::ServerStartEquipWeapon_Implementation()
{
	BP_OnEquipWeapon();
}

void USGPawnComp_CharacterParts::BP_OnEquipWeapon_Implementation()
{
}
