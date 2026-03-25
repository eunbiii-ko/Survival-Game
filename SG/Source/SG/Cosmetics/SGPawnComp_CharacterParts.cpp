// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Cosmetics/SGPawnComp_CharacterParts.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


FSGCharacterPartHandle FSGCharacterPartList::AddEntry(const FSGCharacterPart& NewPart)
{
	// PawnComp의 CharacterPartList가 PartHandle을 관리하고,
	// 이를 ControllerComp_CharacterParts에 전달한다.
	FSGCharacterPartHandle Result;
	Result.PartHandle = PartHandleCounter++;

	// 서버라면, AppliedCharacterPartEntry를 Entries에 추가한다.
	if (ensure(OwnerComp && OwnerComp->GetOwner() && OwnerComp->GetOwner()->HasAuthority()))
	{
		FSGAppliedCharacterPartEntry& NewEntry = Entries.AddDefaulted_GetRef();
		NewEntry.Part = NewPart; // 메타 데이터
		NewEntry.PartHandle = Result.PartHandle;

		// NewEntry를 이용해서 실제 Actor를 생성하고,
		// OwmerComp의 Owner Actor에 Actor끼리 RootComp로 Attach 시킨다.
		if (SpawnActorForEntry(NewEntry))
		{
			
		}

		MarkItemDirty(NewEntry);	
	}

	return Result;
}

bool FSGCharacterPartList::SpawnActorForEntry(FSGAppliedCharacterPartEntry& Entry)
{
	bool bCreatedAnyActor = false;

	if (ensure(OwnerComp) && !OwnerComp->IsNetMode(NM_DedicatedServer))
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
			}
		}
	}
	
	return bCreatedAnyActor;
}

//////////////////////////////////////////////////////////////////////

USGPawnComp_CharacterParts::USGPawnComp_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
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