// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Animation/NotifyState/ANS_SendGameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "SG/AbilitySystem/SGAbilitySystemComponent.h"
#include "SG/Player/SGPlayerState.h"

UANS_SendGameplayEvent::UANS_SendGameplayEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UANS_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp->GetOwner() || !BeginEventTag.IsValid()) return;

	UAbilitySystemComponent* ASC = nullptr;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		ASC = ASI->GetAbilitySystemComponent();	
	}
	else
	{
		// Pawn으로 캐스트
		APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
		if (!Pawn) return;

		// PlayerState에서 ASC 획득
		ASGPlayerState* PS = Pawn->GetPlayerState<ASGPlayerState>();
		if (!PS) return;

		ASC = PS->GetSGAbilitySystemComponent();
	}

	if (!ASC) return;	
	

	ASC->HandleGameplayEvent(BeginEventTag, &EventData);
}

void UANS_SendGameplayEvent::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp->GetOwner() || !TickEventTag.IsValid()) return;

	UAbilitySystemComponent* ASC = nullptr;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		ASC = ASI->GetAbilitySystemComponent();	
	}
	else
	{
		// Pawn으로 캐스트
		APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
		if (!Pawn) return;

		// PlayerState에서 ASC 획득
		ASGPlayerState* PS = Pawn->GetPlayerState<ASGPlayerState>();
		if (!PS) return;

		ASC = PS->GetSGAbilitySystemComponent();
	}

	if (!ASC) return;	
	

	ASC->HandleGameplayEvent(TickEventTag, &EventData);
}

void UANS_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner()) return;

	UAbilitySystemComponent* ASC = nullptr;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		ASC = ASI->GetAbilitySystemComponent();	
	}
	else
	{
		// Pawn으로 캐스트
		APawn* Pawn = Cast<APawn>(MeshComp->GetOwner());
		if (!Pawn) return;

		// PlayerState에서 ASC 획득
		ASGPlayerState* PS = Pawn->GetPlayerState<ASGPlayerState>();
		if (!PS) return;

		ASC = PS->GetSGAbilitySystemComponent();
	}

	if (!ASC) return;	
	

	ASC->HandleGameplayEvent(EndEventTag, &EventData);
}
