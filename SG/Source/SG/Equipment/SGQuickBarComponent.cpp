// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Equipment/SGQuickBarComponent.h"

#include "SGEquipmentDefinition.h"
#include "Net/UnrealNetwork.h"
#include "SG/Equipment/SGEquipmentManagerComponent.h"
#include "SG/Inventory/SGInventoryItemFragment_EquippableItem.h"
#include "SG/Inventory/SGInventoryItemInstance.h"
#include "SG/Equipment/SGEquipmentInstance.h"

USGQuickBarComponent::USGQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void USGQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
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
			OnRep_Slots();
		}
	}
}

USGInventoryItemInstance* USGQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	USGInventoryItemInstance* Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}

	return Result;
}

void USGQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{
	if ((Slots.IsValidIndex(NewIndex)) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
		OnRep_ActiveSlotIndex();
	}
}

USGEquipmentManagerComponent* USGQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<USGEquipmentManagerComponent>();
		}
	}

	return nullptr;
}

void USGQuickBarComponent::UnequipItemInSlot()
{
	// QuickBar는 Controller에 붙어있는 Component지만,
	// EquipmentManagerComp는 Controller가 소유(Possess)하고 있다. 
	if (USGEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		// 현재 장착된 Item이 있다면
		if (EquippedItem)
		{
			// EquipmentManager를 통해, Pawn의 장비를 해제한다.
			EquipmentManager->UnequipItem(EquippedItem);

			// Controller에도 EquipItem의 상태를 없는 것으로 업데이트한다.
			EquippedItem = nullptr;
		}
	}
}

void USGQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 InventoryItemInstance를 찾는다.
	if (USGInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// SlotItem을 통해 (InventoryItemInstance) InventoryFragment_EquippableItem의 Fragment를 찾는다:
		// - 찾는 것이 실패했다면 장착할 수 없는 Inventory Item임을 의미한다.
		if (const USGInventoryItemFragment_EquippableItem* EquipInfo =
			SlotItem->FindFragmentByClass<USGInventoryItemFragment_EquippableItem>())
		{
			// EquippableItem에서 EquipmentDefinition을 찾는다:
			// - EquipmentDefinition이 있어야 장착할 수 있다.
			TSubclassOf<USGEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				// Unequip이랑 비슷하게 EquipmentManger를 통해 장착한다.
				if (USGEquipmentManagerComponent* EquipManager = FindEquipmentManager())
				{
					EquippedItem = EquipManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						// EquippedItem에는 앞서 보았던 Instigator로 Slot을 대상으로 넣는다. 
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
	}
}

void USGQuickBarComponent::OnRep_Slots()
{
}

void USGQuickBarComponent::OnRep_ActiveSlotIndex()
{
}
