// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Weapons/SGGA_Weapon_Attack.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Kismet/KismetMathLibrary.h"
#include "SG/SGGameplayTags.h"
#include "SG/Character/SGCharacter.h"

USGGA_Weapon_Attack::USGGA_Weapon_Attack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGGA_Weapon_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CachedHitActors.Reset();
}

void USGGA_Weapon_Attack::ParseTargetData(const FGameplayAbilityTargetDataHandle& InTargetDataHandle,
	TArray<int32>& OutCharacterHitIndexes, TArray<int32>& OutBlockHitIndexes)
{
	for (int32 i = 0; i < InTargetDataHandle.Data.Num(); i++)
	{
		const TSharedPtr<FGameplayAbilityTargetData>& TargetData = InTargetDataHandle.Data[i];

		if (FHitResult* HitResult = const_cast<FHitResult*>(TargetData->GetHitResult()))
		{
			if (AActor* HitActor = HitResult->GetActor())
			{
				// HitActor가 SGCharacter인지 확인한다.
				ASGCharacter* TargetCharacter = Cast<ASGCharacter>(HitActor);
				if (TargetCharacter == nullptr)
				{
					TargetCharacter = Cast<ASGCharacter>(HitActor->GetOwner());
				}

				// TargetCharacter가 Blocking 상태인지 확인한다. 
				bool bIsCharacterBlockingHit = TargetCharacter ? IsCharacterBlockingHit(TargetCharacter) : false;

				// HitActor가 무기인지 확인한다.
				// AD1EquipmentBase* HitWeaponActor = Cast<AD1EquipmentBase>(HitActor);
				// if (HitWeaponActor && (HitWeaponActor->bCanBlock == false || bIsCharacterBlockingHit == false))
				// 	continue;
				
				// TargetCharacter가 존재하면 TargetCharacter를, 존재하지 않으면 HitActor를 선택한다.
				AActor* SelectedActor = TargetCharacter ? TargetCharacter : HitActor;

				// 이미 Hit 처리된 Actor인지 확인한다. (한 공격에 여러 Hit이 발생할 수 있기 때문)
				if (CachedHitActors.Contains(SelectedActor))
					continue;

				CachedHitActors.Add(SelectedActor);

				if (TargetCharacter)
				{
					bIsCharacterBlockingHit ? OutBlockHitIndexes.Add(i) : OutCharacterHitIndexes.Add(i);
				}
				else
				{
					OutBlockHitIndexes.Add(i);
				}
			}
		}
	}
}

bool USGGA_Weapon_Attack::IsCharacterBlockingHit(ASGCharacter* TargetCharacter)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetCharacter);
	if (TargetASC && TargetASC->HasMatchingGameplayTag(SGGameplayTags::Status_Blocking))
	{
		FVector TargetLocation = TargetCharacter->GetActorLocation();
		FVector TargetDirection = TargetCharacter->GetActorForwardVector();
								
		FVector InstigatorLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
		FVector TargetToInstigator = InstigatorLocation - TargetLocation;
								
		float Degree = UKismetMathLibrary::DegAcos(TargetDirection.Dot(TargetToInstigator.GetSafeNormal()));
		if (Degree <= BlockingAngle)
			return true;
	}
	return false;
}
