// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameModes/AsyncAction_ExperienceReady.h"
#include "SGExperienceManagerComponent.h"
#include "SG/SGLogChannels.h"

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady2(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	// WorldContextObject에 맞는 World를 받아온다. 
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Action = NewObject<UAsyncAction_ExperienceReady>();
		// World를 캐싱한 후,
		Action->WorldPtr = World;
		// World에 Action을 등록한다. 
		Action->RegisterWithGameInstance(World);
	}
	return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	// 캐싱되어 있는 월드를 가져온다.
	if (UWorld* World = WorldPtr.Get())
	{
		// GameState가 이미 World에 준비되어 있으면,
		// Step1을 스킵하고, 바로 Step2를 진행한다.
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}
		else
		{
			// 아직 준비되어 있지 않으면,
			// UWorld::GameStateSetEvent에 Step1_HandleGameStateSet을 Delegate로 바인딩한다.
			//	- GameStateSetEvent: GameState가 생성되면 실행되는 델리게이트 
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}
	else
	{
		// 현재 AsyncAction 대상이었던 World가 더이상 Valid하지 않으므로, 제거한다.
		SetReadyToDestroy();
	}
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		// Activate()에서 바인딩했던 델리게이트를 제거한다.
		World->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	// 함수가 호출될 시점에는 GameState가 반드시 존재해야 한다.
	check(GameState);

	// GameStateSetEvent에 Delegate로 바인딩한 이유:
	//	Experience 로딩 상태 정보가 GameState에 부착된 ExperienceManagerComp에서 가져올 수 있기 때문.
	USGExperienceManagerComponent* ExperienceManagerComp =
		GameState->FindComponentByClass<USGExperienceManagerComponent>();
	check(ExperienceManagerComp);

	// 만약 이미 Experience가 준비되어 있다면, Step3를 스킵하고 Step4로 간다.
	if (ExperienceManagerComp->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);

		World->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateUObject(this, &ThisClass::Step4_BroadcastReady));
	}
	else
	{
		// 준비가 안되었다면, OnExperienceLoaded에 바인딩한다.
		// (나중에 로딩이 완료되면 Step3부터 실행된다.)
		ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(
			FOnSGExperienceLoaded::FDelegate::CreateUObject(
				this, &ThisClass::Step3_HandleExperienceLoaded));
	}
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const USGExperienceDefinition* CurrentExperience)
{
	// 현재 따로 Experience Definition에 대해 처리할 로직은 없다.
	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	// 바인딩된 BP 혹은 UFUNCTION을 호출한다.
	OnReady.Broadcast();
	UE_LOG(LogSG, Display, TEXT("[BC] UAsyncAction_ExperienceReady::Step4_BroadcastReady()"));
	SetReadyToDestroy();
}
