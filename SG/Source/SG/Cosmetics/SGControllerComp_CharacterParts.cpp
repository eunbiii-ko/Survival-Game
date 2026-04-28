// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Cosmetics/SGControllerComp_CharacterParts.h"
#include "SGPawnComp_CharacterParts.h"
#include "SG/SGGameplayTags.h"
#include "SG/SGLogChannels.h"
#include "SG/AbilitySystem/SGTaggedActor.h"
#include "SG/GameModes/SGExperienceManagerComponent.h"

USGControllerComp_CharacterParts::USGControllerComp_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGControllerComp_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(
				this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void USGControllerComp_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	
	Super::EndPlay(EndPlayReason);
}

void USGControllerComp_CharacterParts::ChangeCosmeticPart(FGameplayTag PartTagToChange, const FSGCharacterPart& NewPart)
{
	USGPawnComp_CharacterParts* PawnCustomizer = GetPawnCustomizer();
	if (!PawnCustomizer)
	{
		AddCharacterPart(NewPart);
		return;
	}

	// GetCombinedTags()로 현재 장착된 태그 전체 확인
	FGameplayTagContainer CombinedTags = PawnCustomizer->GetCombinedTags(FGameplayTag());

	// PartTagToChange와 같은 종류(MatchesTag 양방향)의 기존 태그 탐색
	FGameplayTag TagToRemove;
	for (const FGameplayTag& ExistingTag : CombinedTags)
	{
		if (ExistingTag.MatchesTag(PartTagToChange) || PartTagToChange.MatchesTag(ExistingTag))
		{
			TagToRemove = ExistingTag;
			break;
		}
	}

	// 찾은 태그에 해당하는 CharacterParts Entry 제거
	if (TagToRemove.IsValid())
	{
		for (auto EntryIt = CharacterParts.CreateIterator(); EntryIt; ++EntryIt)
		{
			AActor* CosmeticActor = EntryIt->Part.PartClass.GetDefaultObject();
			if (!CosmeticActor) continue;

			ASGTaggedActor* TaggedActor = Cast<ASGTaggedActor>(CosmeticActor);
			if (!TaggedActor) continue;

			if (TaggedActor->CosmeticTag.MatchesTagExact(TagToRemove))
			{
				RemoveCharacterPart(EntryIt->Part);
				break;
			}
		}
	}

	AddCharacterPart(NewPart);

}

void USGControllerComp_CharacterParts::AddCharacterPart(const FSGCharacterPart& NewPart)
{
	UE_LOG(LogSG, Display, TEXT("[메시 장착] USGControllerComp_CharacterParts::AddCharacterPart()"));
	
	AddCharacterPartInternal(NewPart);
}

void USGControllerComp_CharacterParts::AddCharacterPartInternal(const FSGCharacterPart& NewPart)
{
	// AddDefaulted_GetRef():
	// 내부적으로 CharacterParts 크기만큼 할당한 후, 할당된 영역의 포인터를 넘겨준다.
	FSGControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (USGPawnComp_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void USGControllerComp_CharacterParts::RemoveCharacterPart(const FSGCharacterPart& PartToRemove)
{
	for (auto EntryIt = CharacterParts.CreateIterator(); EntryIt; ++EntryIt)
	{
		if (FSGCharacterPart::AreEquivalentParts(EntryIt->Part, PartToRemove))
		{
			if (USGPawnComp_CharacterParts* PawnCustomizer = GetPawnCustomizer())
			{
				PawnCustomizer->RemoveCharacterPart(EntryIt->Handle);
			}

			EntryIt.RemoveCurrent();
			break;
		}
	}
}

void USGControllerComp_CharacterParts::RemoveAllCharacterParts()
{
	if (USGPawnComp_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FSGControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}

	CharacterParts.Reset();
}

USGPawnComp_CharacterParts* USGControllerComp_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<USGPawnComp_CharacterParts>();
	}
	return nullptr;
}

void USGControllerComp_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// 이전 OldPawn에 대한 CharacterParts를 모두 제거한다.
	if (USGPawnComp_CharacterParts* OldCustomizer =
		OldPawn ? OldPawn->FindComponentByClass<USGPawnComp_CharacterParts>() : nullptr)
	{
		for (FSGControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// 새로운 Pawn에 대해서 기존 Controller가 갖고 있는 CharacterPart를 추가한다.
	if (USGPawnComp_CharacterParts* NewCustomizer =
		NewPawn ? NewPawn->FindComponentByClass<USGPawnComp_CharacterParts>() : nullptr)
	{
		for (FSGControllerCharacterPartEntry& Entry : CharacterParts)
		{
			if (!Entry.Handle.IsValid())
			{
				Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
			}
		}
	}

	UE_LOG(LogSG, Display, TEXT("[메시 장착] USGControllerComp_CharacterParts::OnPossessedPawnChanged() in %d"), NewPawn->HasAuthority());
}