// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Equipment/SGQuickBarComponent.h"

USGQuickBarComponent::USGQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USGQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	
	Super::BeginPlay();
}

void USGQuickBarComponent::AddItemToSlot(int32 SlotIndex, USGInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			// 동적 추가가 아니라 바로 넣는다. 
			Slots[SlotIndex] = Item;
		}
	}
}
