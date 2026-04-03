// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Equipment/SGEquipmentInstance.h"

#include "SGEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#include "Net/UnrealNetwork.h"
#include "SG/SGLogChannels.h"

USGEquipmentInstance::USGEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void USGEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

UWorld* USGEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

APawn* USGEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void USGEquipmentInstance::SpawnEquipmentActors(const TArray<FSGEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		// 현재 Owner인 Pawn의 RootComp를 AttachTarget 대상으로 한다.
		USceneComponent* AttachToTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Character = Cast<ACharacter>(OwningPawn))
		{
			// 만약 캐릭터라면, SkeletalMeshComp가 있으면 GetMesh()로 반환하여 여기에 붙인다.
			AttachToTarget = Character->GetMesh();
		}

		for (const FSGEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			// SpawnActorDeferred는 FinishSpawning()을 호출해야 Spawn이 완료된다.
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(
				SpawnInfo.ActorToSpawn,
				FTransform::Identity,
				OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);

			// Actor의 RelativeTransform을 AttachTransform으로 설정한다.
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			// AttachTarget에 붙인다.
			NewActor->AttachToComponent(AttachToTarget,
				FAttachmentTransformRules::KeepRelativeTransform,
				SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);

			UE_LOG(LogSG, Display, TEXT("[무기 장착] USGEquipmentInstance::SpawnEquipmentActors() in %d"), OwningPawn->HasAuthority());	
		}
	}
}

void USGEquipmentInstance::DestroyEquipmentActors()
{
	// 장착물이 복수 개의 Actor Mesh로 구성되어 있을 수 있다.
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void USGEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void USGEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void USGEquipmentInstance::OnRep_Instigator()
{
}
