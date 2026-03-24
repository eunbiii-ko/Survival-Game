// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameFeatures/GFA_AddInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "GameFeaturesSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "SG/Input/SGMappableConfigPair.h"
#include "PlayerMappableInputConfig.h"
#include "SG/Character/SGHeroInputComponent.h"

void UGFA_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// 안전성을 위해 한번 더 체크한다.
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) ||
		!ensure(ActiveData.PawnsAddedTo.IsEmpty()))
	{
		Reset(ActiveData);
	}

	// UGFAction_WorldActionBase를 호출하면서, AddToWorld()를 호출한다.
	Super::OnGameFeatureActivating(Context);
}

void UGFA_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UGFA_AddInputConfig::AddToworld(const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (GameInstance && World && World->IsGameWorld())
	{
		// GFCM를 이용하여 ExtensionHandler를 추가하여 등록한다.
		if (UGameFrameworkComponentManager* CompManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			// 추가적인 이벤트 요청이 오면 HandlePawnExtension을 호출하라는 델리게이트 
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this,
					&ThisClass::HandlePawnExtension, ChangeContext);

			// 등록된 콜백 함수의 핸들을 ActiveData의 ExtensionRequestHandles에 등록한다.
			// AddExtensionHandler(): 모든 Pawn에 콜이 오면 AddConfigDelegate를 호출하도록 설정한다.   
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle =
				CompManager->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);
			ActiveData.ExtensionRequestHandles.Add(ExtensionRequestHandle);
		}
	}
}

void UGFA_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName,
	FGameFeatureStateChangeContext ChangeContext)
{
	APawn* Pawn = CastChecked<APawn>(Actor);
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded ||
		EventName == USGHeroInputComponent::NAME_BindInputsNow)
	{
		AddInputConfig(Pawn, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved)
	{
		RemoveInputConfig(Pawn, ActiveData);
	}
}

void UGFA_AddInputConfig::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PC ? PC->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;
			
			// 추가된 InputConfigs를 순회하며,
			// EnhancedInputSubsystem에 PlayerMappableConfig를 추가한다.
			for (const FSGMappableConfigPair& Pair : InputConfigs)
			{
				if (Pair.bShouldActivateAutomatically)
				{
					Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
				}
			}

			// ActiveData에 Pawn을 관리 대상으로 등록한다.(캐싱해둔다.)
			ActiveData.PawnsAddedTo.AddUnique(Pawn);
		}
	}
}

void UGFA_AddInputConfig::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PC ? PC->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// InputConfigs를 순회하며 Config를 제거한다.
			for (const FSGMappableConfigPair& Pair : InputConfigs)
			{
				Subsystem->RemovePlayerMappableConfig(Pair.Config.LoadSynchronous());
			}

			// 캐싱된 Pawn도 삭제한다.
			ActiveData.PawnsAddedTo.Remove(Pawn);
		}
	}
}

void UGFA_AddInputConfig::Reset(FPerContextData& ActiveData)
{
	// ExtensionRequestHandles 초기화
	ActiveData.ExtensionRequestHandles.Empty();

	// PawnsAddedTo에 대해서 하나씩 Stack 방식으로 위에서 아래로 직접 InputConfig를 제거한다.
	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		// PawnsAddedTo을 전부 순회하면서 하나씩 가져온다.
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();
		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}
		else
		{
			// WeakObjectPtr로 PawnsAddedTo을 관리하고 있기 때문에
			// GC 되었다면 nullptr일 수 있음.
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}
