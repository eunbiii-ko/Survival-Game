// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGInventoryItemInstance.generated.h"

class USGInventoryItemFragment;
class USGInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class SG_API USGInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:
	USGInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const USGInventoryItemFragment* FindFragmentByClass(TSubclassOf<USGInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
	
	/**
	 * Inventory Item의 인스턴스에는 무엇으로 정의되어 있는지
	 * 메타 클래스인 LccInventoryItemDefinition을 들고 있다.
	 */
	UPROPERTY()
	TSubclassOf<USGInventoryItemDefinition> ItemDefinition;
};
