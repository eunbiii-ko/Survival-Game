// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Inventory/SGInventoryItemDefinition.h"

USGInventoryItemDefinition::USGInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const USGInventoryItemFragment* USGInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<USGInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		// Fragments를 순회하며, IsA()를 통해 해당 클래스를 가지고 있는지 확인한다.
		for (USGInventoryItemFragment* Fragment : Fragments)
		{
			// IsA(): 형변환 가능 여부를 판단한다. 
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
