// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroComponent.h"
#include "SGPawnExtensionComponent.h"
#include "SG/SGGameplayTags.h"
#include "SG/SGLogChannels.h"

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
	IGameFrameworkInitStateInterface::OnActorInitStateChanged(Params);
}

bool USGHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return IGameFrameworkInitStateInterface::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void USGHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	IGameFrameworkInitStateInterface::HandleChangeInitState(Manager, CurrentState, DesiredState);
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
