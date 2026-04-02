// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "SGQuickBarComponent.generated.h"

class USGInventoryItemInstance;
class USGEquipmentInstance;
class USGEquipmentManagerComponent;

/**
 * 해당 컴포넌트는 ControllerComp로서, PC에 의해 조작계 중 하나로 생각해도 된다.
 *  - HUD(Slate)와 Inventory/Equipment(Gameplay)의 다리(Bridge) 역할을 하는 Component
 */
UCLASS()
class SG_API USGQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	USGQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * ControllerComponent interface
	 */
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, USGInventoryItemInstance* Item);
	UFUNCTION(BlueprintCallable)
	void SetActiveSlotIndex(int32 NewIndex);

	USGEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();
	
	/** HUD QuickBar Slot 개수 */
	UPROPERTY()
	int32 NumSlots = 3;

	/** HUD QuickBar에 장착된 Slot 리스트 */
	UPROPERTY()
	TArray<TObjectPtr<USGInventoryItemInstance>> Slots;

	/** 현재 활성화된 Slot Index (딱 하나의 Slot이 활성화됨) */
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	/** 현재 장착한 장비 정보 */
	UPROPERTY()
	TObjectPtr<USGEquipmentInstance> EquippedItem;
};
