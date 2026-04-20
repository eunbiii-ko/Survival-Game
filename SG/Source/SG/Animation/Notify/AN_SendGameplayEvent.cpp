// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Animation/Notify/AN_SendGameplayEvent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagsManager.h"
#include "SG/AbilitySystem/SGAbilitySystemComponent.h"
#include "SG/Player/SGPlayerState.h"

void UAN_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

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
	

	FGameplayEventData EventData;
	ASC->HandleGameplayEvent(EventTag, &EventData);
}

FString UAN_SendGameplayEvent::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		TArray<FName> TagNames;
		UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
		return TagNames.Last().ToString();
	}
	return "None";
}
