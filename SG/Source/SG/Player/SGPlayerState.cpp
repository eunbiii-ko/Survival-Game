// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Player/SGPlayerState.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"
#include "SG/AbilitySystem/SGAbilitySet.h"
#include "SG/AbilitySystem/SGAbilitySystemComponent.h"
#include "SG/Character/SGPawnData.h"
#include "SG/GameModes/SGExperienceManagerComponent.h"
#include "SG/GameModes/SGGameMode.h"


ASGPlayerState::ASGPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComp = ObjectInitializer.CreateDefaultSubobject<USGAbilitySystemComponent>
							(this, TEXT("AbilitySystemComp"));
}

void ASGPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComp);
	{
		// 처음 InitAbilityActorInfo()를 호출 당시, OwnerActor와 AvatarActor가 같은 Actor를 가리키고 있으며,
		// 이는 PlayerState이다.
		// - OwnerActor는 PlayerState가 의도하는게 맞지만,
		//	 AvatarActor는 PlayerController가 소유하는 대상인 Pawn이 되어야 한다.
		// 이를 위해 아래에서 InitAbilityActorInfo()를 다시 호출해서 세팅한다.  
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComp->AbilityActorInfo.Get(); 
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);
	}
	// 여기서 GetPawn()은 현재 nullptr이다. 
	AbilitySystemComp->InitAbilityActorInfo(this, GetPawn());
	
	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);

		USGExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<USGExperienceManagerComponent>();
		check(ExperienceManagerComp);

		ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(
			FOnSGExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ASGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
}

void ASGPlayerState::OnExperienceLoaded(const USGExperienceDefinition* CurrentExperience)
{
	if (ASGGameMode* GameMode = GetWorld()->GetAuthGameMode<ASGGameMode>())
	{
		const USGPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ASGPlayerState::SetPawnData(const USGPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	
	// PawnData가 2번 중복으로 설정되는 것을 막기 위함
	check(!PawnData);

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// PawnData의 AbilitySets을 순회하며, ASC에 Ability를 할당한다.
	for (USGAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComp, nullptr);
		}
	}
	
	ForceNetUpdate();
}

void ASGPlayerState::OnRep_PawnData()
{
}
