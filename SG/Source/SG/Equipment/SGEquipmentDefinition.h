// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGEquipmentDefinition.generated.h"

class USGEquipmentInstance;

//////////////////////////////////////////////////////////////////////

USTRUCT()
struct FSGEquipmentActorToSpawn
{
	GENERATED_BODY()

	/** Spawn할 대상 Actor 클래스 (Actor를 상속받은 Asset) */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;
	
	/** 어느 Bone Socket에 붙일지 결정한다. */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket;
	
	/** Socket에서 어느정도 Transformation을 더할 것인지 결정한다. */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
};

//////////////////////////////////////////////////////////////////////

/**
 * 장착 아이템에 대한 정의 클래스(메타 데이터)
 */
UCLASS(Blueprintable)
class SG_API USGEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	USGEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** 해당 메타 데이터를 사용하면, 어떤 인스턴스를 Spawn할지 결정하는 클래스 */
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<USGEquipmentInstance> InstanceType;

	/** 해당 장착 아이템을 사용하면, 어떤 Actor가 Spawn되는지 정보를 담고 있다. */
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FSGEquipmentActorToSpawn> ActorsToSpawn;
};
