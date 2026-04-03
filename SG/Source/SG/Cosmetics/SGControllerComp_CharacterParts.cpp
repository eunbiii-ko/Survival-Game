// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Cosmetics/SGControllerComp_CharacterParts.h"
#include "SGPawnComp_CharacterParts.h"
#include "SG/SGLogChannels.h"
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

		AGameStateBase* GameState = GetWorld()->GetGameState();
		if (USGExperienceManagerComponent* ExpComp = GameState->FindComponentByClass<USGExperienceManagerComponent>())
		{
			ExpComp->CallOrRegister_OnExperienceLoaded(
				FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
		}
	}
}

void USGControllerComp_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	
	Super::EndPlay(EndPlayReason);
}

void USGControllerComp_CharacterParts::AddCharacterPart(const FSGCharacterPart& NewPart)
{
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

void USGControllerComp_CharacterParts::OnExperienceLoaded(const USGExperienceDefinition* LoadedExperienceDefinition)
{
	if (AController* OwningController = GetController<AController>())
	{
		OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);

		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			OnPossessedPawnChanged(nullptr, ControlledPawn);
		}
	}
}
