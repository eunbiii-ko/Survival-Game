// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Net/UnrealNetwork.h"
#include "SG/SGGameplayTags.h"
#include "SG/SGLogChannels.h"

const FName USGPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

USGPawnExtensionComponent::USGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
}

bool USGPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	// InitState_Spawned 초기화
	if (!CurrentState.IsValid() && DesiredState == SGGameplayTags::InitState_Spawned)
	{
		// Pawn이 잘 세팅만 되어 있으면 바로 InitState_Spawned로 넘어간다. 
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == SGGameplayTags::InitState_Spawned && DesiredState == SGGameplayTags::InitState_DataAvailable)
	{
		// 아마 PawnData를 누군가 설정하는 모양이다.
		if (!PawnData)
		{
			return false;
		}

		// LocallyControlled인 Pawn이 Controller가 없으면 에러!
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bHasAuthority = Pawn->HasAuthority();
		if (bIsLocallyControlled || bHasAuthority)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}

		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		// Actor에 바인드된 모든 Feature들이 DataAvailable 상태일 때, DataInitialized로 넘어갈 수 있다.
		return Manager->HaveAllFeaturesReachedInitState(Pawn, SGGameplayTags::InitState_DataAvailable);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == SGGameplayTags::InitState_DataInitialized && DesiredState == SGGameplayTags::InitState_GameplayReady)
	{
		UE_LOG(LogSG, Display, TEXT("USGPawnExtensionComponent::CanChangeInitState() DataInitialized -> GameplayReady %s in %d name: %s"),
			*GetName(), Pawn->HasAuthority(), *Pawn->GetName());
		return true;
	}

	// 위의 선형적인 transition이 아니면 false
	return false;
}

void USGPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// PawnExtComp는 다른 Feature Comp들의 상태가 DataAvailable를 관찰하여,
		// CanChangeInitState()에서 Sync를 맞춘다.
		// - 이를 가능하게 하기 위해, OnActorInitStateChanged()에서 DataAvailable에 대해
		//   지속적으로 CheckDefaultInitialization()를 호출하여 상태를 확인한다. 
		if (Params.FeatureState == SGGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void USGPawnExtensionComponent::SetPawnData(const USGPawnData* InPawnData)
{
	check(InPawnData);

	APawn* Pawn = GetPawnChecked<APawn>();

	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	CheckDefaultInitialization();
}

void USGPawnExtensionComponent::SetupPlayerInputComponent()
{
	// ForceUpdate로 다시 InitState 상태 변환 시작
	CheckDefaultInitialization();
}

void USGPawnExtensionComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void USGPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

void USGPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Pawn에 등록되었는지 확인한다.
	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr),
		TEXT("LyraPawnExtensionComponent on [%s] can only be added to Pawn actors."),
		*GetNameSafe(GetOwner()));

	UE_LOG(LogSG, Display, TEXT("%s OnRegister in %d"), *NAME_ActorFeatureName.ToString(), Pawn ? Pawn->HasAuthority() : -1);

	// GameFrameworkComponentManager에 InitState 사용을 위해 등록을 진행한다.
	RegisterInitStateFeature();
}

void USGPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// NAME_None: Actor에 등록된 Feature Component의 InitState 상태를 관찰하겠다는 의미이다.
	// FGameplayTag(): 모든 상태 변화에 대해 관찰한다. 
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// 현재 상태를 InitState_Spawned로 변화한다.
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

void USGPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// OnRegister()의 RegisterInitStateFeature()의 쌍을 맞추어준다.
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void USGPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USGPawnExtensionComponent, PawnData);
}

void USGPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void USGPawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComp는 Feature Comp들의 초기화를 관장하는 Comp이다.
	// - 따라서, Actor에 바인딩된 FeatureComp들에 대해 
	//   CheckDefaultInitialization()을 호출해주도록 한다.
	//   (ForceUpdate 느낌)
	// - 이 메서드를 IGameFrameworkInitStateInterface가 제공하는데, CheckDefaultInitializationForImplementers() 이다. 
	CheckDefaultInitializationForImplementers();

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
