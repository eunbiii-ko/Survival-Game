// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroComponent.h"
#include "SGPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "SG/SGGameplayTags.h"
#include "SG/SGLogChannels.h"
#include "SG/Player/SGPlayerController.h"
#include "SG/Player/SGPlayerState.h"

const FName USGHeroComponent::NAME_ActorFeatureName("Hero");

USGHeroComponent::USGHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USGHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Pawn에 등록되었는지 확인한다. 
	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr),
		TEXT("SGHeroComponent on [%s] can only be added to Pawn actors."),
		*GetNameSafe(GetOwner()));

	
	UE_LOG(LogSG, Display, TEXT("%s OnRegister in %d"), *GetFeatureName().ToString(), HasAuthority());

	// GameFrameworkComponentManager에 InitState 사용을 위해 등록을 진행한다.
	RegisterInitStateFeature();
}

void USGHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComp에 대해서 OnActorInitStateChanged() 관찰하도록 설정한다.
	BindOnActorInitStateChanged(USGPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 상태 변환을 시도한다.
	//   1. CanChangeInitState(): 상태 변환 가능성 유무 판단
	//   2. HandleChangeInitState(): 내부 상태 변경 (Feature Component)
	//   3. BindOnActorInitStateChanged(): Bind된 Delegate를 조건에 맞게 호출한다.
	//      - SGPawnExtensionComp의 경우, 모든 Actor의 Feature 상태 변화에 대해 OnActorInitStateChanged()가 호출된다.
	ensure(TryToChangeInitState(SGGameplayTags::InitState_Spawned));

	// 해당 함수는 우리가 오버라이딩한다.
	// - 이 함수를 ForceUpdateInitState()와 같은 느낌으로 이해하면 된다.
	// - 현재 강제 업데이트 진행 (물론 CanChangeInitState와 HandleChangeInitState를 진행한다.)
	CheckDefaultInitialization();
}

void USGHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// OnRegister()의 RegisterInitStateFeature()의 쌍을 맞추어준다.
	UnregisterInitStateFeature();
	
	Super::EndPlay(EndPlayReason);
}

void USGHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == USGPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// PawnExtComp의 DataInitialized 상태 변화 관찰 후,
		// HeroComp도 DataInitialized 상태로 변경한다. (자신의 업데이트 호출)
		// - CanChangeInitState() 확인 
		if (Params.FeatureState == SGGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool USGHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	ASGPlayerState* SGPs = GetPlayerState<ASGPlayerState>();

	// Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == SGGameplayTags::InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == SGGameplayTags::InitState_Spawned && DesiredState == SGGameplayTags::InitState_DataAvailable)
	{
		if (!SGPs)
		{
			return false;
		}
		
		// 서버 또는 본인 캐릭터라면 PlayerState가 제대로 설정되어 있는지 확인한다.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);
		
			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}
		
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();
		
		if (bIsLocallyControlled && !bIsBot)
		{
			ASGPlayerController* SGPC = GetController<ASGPlayerController>();
		
			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !SGPC || !SGPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		// PawnExtComp가 DataInitialized 상태에 도달했는지 확인한다.
		// == 모든 FeatureComp들이 DataAvailable인 상태가 됐는지 확인한다.
		return SGPs && Manager->HasFeatureReachedInitState(Pawn,
			USGPawnExtensionComponent::NAME_ActorFeatureName,
			SGGameplayTags::InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == SGGameplayTags::InitState_DataInitialized && DesiredState == SGGameplayTags::InitState_GameplayReady)
	{
		UE_LOG(LogSG, Display, TEXT("USGHeroComponent::CanChangeInitState() DataInitialized -> GameplayReady %s in %d, name: %s"),
			*GetName(), Pawn->HasAuthority(), *Pawn->GetName());
		return true;
	}
	
	return false;
}

void USGHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	// DataAvailable -> DataInitialized
	if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ASGPlayerState* LccPs = GetPlayerState<ASGPlayerState>();
		if (!ensure(Pawn && LccPs))
		{
			return;
		}

		// todo
		//  - Input과 Camera에 대한 핸들링
		
		const bool bIIsLocallyControlled = Pawn->IsLocallyControlled();
		const USGPawnData* PawnData = nullptr;
		if (USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<USGPawnData>();
		}
	}
}

void USGHeroComponent::CheckDefaultInitialization()
{
	// 사용자 정의 InitState를 직접 넘겨줘야 한다.
	static const TArray<FGameplayTag> StateChain =
		{
		SGGameplayTags::InitState_Spawned,
		SGGameplayTags::InitState_DataAvailable,
		SGGameplayTags::InitState_DataInitialized,
		SGGameplayTags::InitState_GameplayReady
	};

	// CanChangeInitState(), HandleChangeInitState(), ChangeFeatureInitState(), OnActorInitStateChanged() 델리게이트 호출까지 진행
	ContinueInitStateChain(StateChain);
}
