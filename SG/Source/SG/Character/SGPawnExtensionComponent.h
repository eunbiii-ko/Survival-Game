// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "SG/Character/SGPawnData.h"
#include "SGPawnExtensionComponent.generated.h"

class USGAbilitySystemComponent;
class USGPawnData;
/**
 * 컴포넌트들의 초기화를 담당한다.
 */
UCLASS()
class SG_API USGPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	USGPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	/** FeatureName 정의 */
	static const FName NAME_ActorFeatureName;
	
	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override final;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override final;
	virtual void CheckDefaultInitialization() override final;
	//~ End IGameFrameworkInitStateInterface interface

	/**
	 * member methods
	 */
	static USGPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		return (Actor ? Actor->FindComponentByClass<USGPawnExtensionComponent>() : nullptr);
	}
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const USGPawnData* InPawnData);

	void SetupPlayerInputComponent();
	
	/** Pawn의 Controller가 변경될 때, Pawn을 소유한 쪽에서 호출된다. */
    void HandleControllerChanged();
	/** PlayerState가 복제됐을 때, Pawn을 소유한 쪽에서 호출된다. */
	void HandlePlayerStateReplicated();
	/** AbilitySystemComponent의 AvatarActor 대상 초기화/해제 */
	void InitializeAbilitySystem(USGAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitalizeAbilitySystem();
	
protected:
	virtual void OnRegister() override final;
	virtual void BeginPlay() override final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_PawnData();
	
	/** Pawn data used to create the pawn. Specified from a spawn function or on a placed instance. */
	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "SG | Pawn")
	TObjectPtr<const USGPawnData> PawnData;

	/** ASC를 캐싱해둔다. */
	UPROPERTY()
	TObjectPtr<USGAbilitySystemComponent> AbilitySystemComp;
};


