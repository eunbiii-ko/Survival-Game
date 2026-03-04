// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/SGGameMode.h"

#include "SGExperienceDefinition.h"
#include "SGExperienceManagerComponent.h"
#include "SGGameState.h"
#include "SG/SGLogChannels.h"
#include "SG/Character/SGCharacter.h"
#include "SG/Character/SGPawnData.h"
#include "SG/Player/SGPlayerController.h"
#include "SG/Player/SGPlayerState.h"

ASGGameMode::ASGGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ASGGameState::StaticClass();
	PlayerControllerClass = ASGPlayerController::StaticClass();
	PlayerStateClass = ASGPlayerState::StaticClass();
	DefaultPawnClass = ASGCharacter::StaticClass();
}

void ASGGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해, 초기화 작업이 진행되므로,
	// 현 프레임에는 Lyra의 Concept인 Experience 처리를 진행할 수 없다.
	// -> 이를 처리하기 위해, 한 프레임 뒤에 이벤트를 받아 처리를 이어서 진행한다.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ASGGameMode::InitGameState()
{
	Super::InitGameState();

	USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);

	// 비동기 로딩 완료에 대한 델리게이트에 함수를 바인딩한다.
	ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(
		FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ASGGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// GetPawnDataForController() 함수를 이용해서 PawnData로부터 PawnClass를 가져온다.
	if (const USGPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASGGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Experience 로드가 완료된 후, PlayerController의 Pawn 스폰이 진행되도록 한다.
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ASGGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	UE_LOG(LogSG, Display, TEXT("[ASGGameMode::SpawnDefaultPawnAtTransform_Implementation]"));
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

const USGPawnData* ASGGameMode::GetPawnDataForController(const AController* InController) const
{
	// 게임 도중에 PawnData가 오버라이드 되었을 경우, PawnData는 PlayerState에서 가져온다.
	if (InController)
	{
		if (const ASGPlayerState* LccPs = InController->GetPlayerState<ASGPlayerState>())
		{
			if (const USGPawnData* PawnData = LccPs->GetPawnData<USGPawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우,
	// ExperienceManagerComp의 CurrentExperience로부터 가져와서 설정한다.
	check(GameState);

	USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);

	// 안전성 목적 검사: 로딩 완료 여부 
	if (ExperienceManagerComp->IsExperienceLoaded())
	{
		// 안전성 목적 검사: CurrentExperience 존재 여부
		const USGExperienceDefinition* Experience = ExperienceManagerComp->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 없으면 nullptr
	return nullptr;
}

void ASGGameMode::OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience)
{
}

bool ASGGameMode::IsExperienceLoaded() const
{
	check(GameState);
	USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);

	return ExperienceManagerComp->IsExperienceLoaded();
}

void ASGGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// 로딩할 Experience에 대해 PrimaryAssetID를 생성하여,
	// OnMatchAssignmentGiven으로 넘겨준다.

	FPrimaryAssetId ExperienceID;
	UWorld* World = GetWorld();

	// 일단 기본 옵션으로 B_LccDefaultExperience로 설정한다.
	if (!ExperienceID.IsValid())
	{
		ExperienceID = FPrimaryAssetId(FPrimaryAssetType("SGExperienceDefinition"), FName("SGDefaultExperience"));
		UE_LOG(LogSG, Display, TEXT("Selected ExperienceID: %s (IsValid=%d)"), *ExperienceID.ToString(), ExperienceID.IsValid());
	}

	// 
	OnMatchAssignmentGiven(ExperienceID);
}

void ASGGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceID)
{
	// ExperienceManagerComp를 활용하여 Experience를 로딩하기 위해,
	// ExperienceManagerComp의 ServerSetCurrentExperience를 호출한다.
	check(ExperienceID.IsValid());

	USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);
	ExperienceManagerComp->ServerSetCurrentExperience(ExperienceID);
}